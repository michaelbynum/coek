import sys
import logging
from weakref import ref as weakref_ref
from typing import overload

from pyomo.common.log import is_debug_set
from pyomo.common.modeling import NOTSET
from pyomo.common.deprecation import RenamedClass
from pyomo.common.formatting import tabular_writer
from pyomo.common.timing import ConstructionTimer
from pyomo.core.expr.numvalue import value
from pyomo.core.base.component import ActiveComponentData, ModelComponentFactory
from pyomo.core.base.global_set import UnindexedComponent_index
from pyomo.core.base.indexed_component import (
    ActiveIndexedComponent,
    UnindexedComponent_set,
    rule_wrapper,
)
from pyomo.core.base.set import Set
from pyomo.core.base.initializer import (
    Initializer,
    IndexedCallInitializer,
    CountedCallInitializer,
)
from pyomo.core.base import minimize, maximize
from pyomo.core.base.objective import _ObjectiveData, _rule_returned_none_error
import poek as pk
from pyomo.core.base.initializer import initializer_map, ConstantInitializer

logger = logging.getLogger(__name__)
initializer_map[pk.expression] = ConstantInitializer


class _GeneralObjectiveData(ActiveComponentData):

    __slots__ = ("_expr", "_sense")

    def __init__(self, expr=None, sense=minimize, component=None):
        super().__init__(component)
        self._expr = expr
        self._sense = sense

        if (self._sense != minimize) and (self._sense != maximize):
            raise ValueError(
                "Objective sense must be set to one of "
                "'minimize' (%s) or 'maximize' (%s). Invalid "
                "value: %s'" % (minimize, maximize, sense)
            )

    @property
    def expr(self):
        return self._expr

    @expr.setter
    def expr(self, val):
        self._expr = val

    def set_value(self, expr):
        if expr is None:
            raise ValueError(_rule_returned_none_error % (self.name,))
        self._expr = expr

    @property
    def sense(self):
        """Access sense (direction) of this objective."""
        return self._sense

    @sense.setter
    def sense(self, sense):
        """Set the sense (direction) of this objective."""
        self.set_sense(sense)

    def set_sense(self, sense):
        """Set the sense (direction) of this objective."""
        if sense in {minimize, maximize}:
            self._sense = sense
        else:
            raise ValueError(
                "Objective sense must be set to one of "
                "'minimize' (%s) or 'maximize' (%s). Invalid "
                "value: %s'" % (minimize, maximize, sense)
            )

    def is_minimizing(self):
        return self.sense == minimize


@ModelComponentFactory.register("Expressions that are minimized or maximized.")
class Objective(ActiveIndexedComponent):
    """
    This modeling component defines an objective expression.

    Note that this is a subclass of NumericValue to allow
    objectives to be used as part of expressions.

    Constructor arguments:
        expr
            A Pyomo expression for this objective
        rule
            A function that is used to construct objective expressions
        sense
            Indicate whether minimizing (the default) or maximizing
        name
            A name for this component
        doc
            A text string describing this component

    Public class attributes:
        doc
            A text string describing this component
        name
            A name for this component
        active
            A boolean that is true if this component will be used to construct
            a model instance
        rule
            The rule used to initialize the objective(s)
        sense
            The objective sense

    Private class attributes:
        _constructed
            A boolean that is true if this component has been constructed
        _data
            A dictionary from the index set to component data objects
        _index
            The set of valid indices
        _implicit_subsets
            A tuple of set objects that represents the index set
        _model
            A weakref to the model that owns this component
        _parent
            A weakref to the parent block that owns this component
        _type
            The class type for the derived subclass
    """

    _ComponentDataClass = _GeneralObjectiveData
    NoObjective = ActiveIndexedComponent.Skip

    def __new__(cls, *args, **kwds):
        if cls != Objective:
            return super(Objective, cls).__new__(cls)
        if not args or (args[0] is UnindexedComponent_set and len(args) == 1):
            return ScalarObjective.__new__(ScalarObjective)
        else:
            return IndexedObjective.__new__(IndexedObjective)

    @overload
    def __init__(self, *indexes, expr=None, rule=None, sense=minimize, name=None, doc=None):
        ...

    def __init__(self, *args, **kwargs):
        _sense = kwargs.pop("sense", minimize)
        _init = tuple(
            _arg
            for _arg in (kwargs.pop("rule", None), kwargs.pop("expr", None))
            if _arg is not None
        )
        if len(_init) == 1:
            _init = _init[0]
        elif not _init:
            _init = None
        else:
            raise ValueError(
                "Duplicate initialization: Objective() only " "accepts one of 'rule=' and 'expr='"
            )

        kwargs.setdefault("ctype", Objective)
        ActiveIndexedComponent.__init__(self, *args, **kwargs)

        self.rule = Initializer(_init)
        self._init_sense = Initializer(_sense)

    def construct(self, data=None):
        """
        Construct the expression(s) for this objective.
        """
        if self._constructed:
            return
        self._constructed = True

        timer = ConstructionTimer(self)
        if is_debug_set(logger):
            logger.debug("Constructing objective %s" % (self.name))

        rule = self.rule
        try:
            # We do not (currently) accept data for constructing Objectives
            index = None
            assert data is None

            if rule is None:
                # If there is no rule, then we are immediately done.
                return

            if rule.constant() and self.is_indexed():
                raise IndexError(
                    "Objective '%s': Cannot initialize multiple indices "
                    "of an objective with a single expression" % (self.name,)
                )

            block = self.parent_block()
            if rule.contains_indices():
                # The index is coming in externally; we need to validate it
                for index in rule.indices():
                    ans = self.__setitem__(index, rule(block, index))
                    if ans is not None:
                        self[index].set_sense(self._init_sense(block, index))
            elif not self.index_set().isfinite():
                # If the index is not finite, then we cannot iterate
                # over it.  Since the rule doesn't provide explicit
                # indices, then there is nothing we can do (the
                # assumption is that the user will trigger specific
                # indices to be created at a later time).
                pass
            else:
                # Bypass the index validation and create the member directly
                for index in self.index_set():
                    ans = self._setitem_when_not_present(index, rule(block, index))
                    if ans is not None:
                        ans.set_sense(self._init_sense(block, index))
        except Exception:
            err = sys.exc_info()[1]
            logger.error(
                "Rule failed when generating expression for "
                "Objective %s with index %s:\n%s: %s"
                % (self.name, str(index), type(err).__name__, err)
            )
            raise
        finally:
            timer.report()

    def _getitem_when_not_present(self, index):
        if self.rule is None:
            raise KeyError(index)

        block = self.parent_block()
        obj = self._setitem_when_not_present(index, self.rule(block, index))
        if obj is None:
            raise KeyError(index)
        obj.set_sense(self._init_sense(block, index))

        return obj

    def _pprint(self):
        """
        Return data that will be printed for this component.
        """
        return (
            [
                ("Size", len(self)),
                ("Index", self._index_set if self.is_indexed() else None),
                ("Active", self.active),
            ],
            self._data.items(),
            ("Active", "Sense", "Expression"),
            lambda k, v: [v.active, ("minimize" if (v.sense == minimize) else "maximize"), v.expr],
        )

    def display(self, prefix="", ostream=None):
        """Provide a verbose display of this object"""
        if not self.active:
            return
        tab = "    "
        if ostream is None:
            ostream = sys.stdout
        ostream.write(prefix + self.local_name + " : ")
        ostream.write(
            ", ".join(
                "%s=%s" % (k, v)
                for k, v in [
                    ("Size", len(self)),
                    ("Index", self._index_set if self.is_indexed() else None),
                    ("Active", self.active),
                ]
            )
        )

        ostream.write("\n")
        tabular_writer(
            ostream,
            prefix + tab,
            ((k, v) for k, v in self._data.items() if v.active),
            ("Active", "Value"),
            lambda k, v: [
                v.active,
                value(v),
            ],
        )


class ScalarObjective(_GeneralObjectiveData, Objective):
    """
    ScalarObjective is the implementation representing a single,
    non-indexed objective.
    """

    def __init__(self, *args, **kwd):
        _GeneralObjectiveData.__init__(self, expr=None, component=self)
        Objective.__init__(self, *args, **kwd)
        self._index = UnindexedComponent_index

    #
    # Since this class derives from Component and
    # Component.__getstate__ just packs up the entire __dict__ into
    # the state dict, we do not need to define the __getstate__ or
    # __setstate__ methods.  We just defer to the super() get/set
    # state.  Since all of our get/set state methods rely on super()
    # to traverse the MRO, this will automatically pick up both the
    # Component and Data base classes.
    #

    #
    # Override abstract interface methods to first check for
    # construction
    #

    @property
    def expr(self):
        """Access the expression of this objective."""
        if self._constructed:
            if len(self._data) == 0:
                raise ValueError(
                    "Accessing the expression of ScalarObjective "
                    "'%s' before the Objective has been assigned "
                    "a sense or expression. There is currently "
                    "nothing to access." % (self.name)
                )
            return _GeneralObjectiveData.expr.fget(self)
        raise ValueError(
            "Accessing the expression of objective '%s' "
            "before the Objective has been constructed (there "
            "is currently no value to return)." % (self.name)
        )

    @expr.setter
    def expr(self, expr):
        """Set the expression of this objective."""
        self.set_value(expr)

    @property
    def sense(self):
        """Access sense (direction) of this objective."""
        if self._constructed:
            if len(self._data) == 0:
                raise ValueError(
                    "Accessing the sense of ScalarObjective "
                    "'%s' before the Objective has been assigned "
                    "a sense or expression. There is currently "
                    "nothing to access." % (self.name)
                )
            return _GeneralObjectiveData.sense.fget(self)
        raise ValueError(
            "Accessing the sense of objective '%s' "
            "before the Objective has been constructed (there "
            "is currently no value to return)." % (self.name)
        )

    @sense.setter
    def sense(self, sense):
        """Set the sense (direction) of this objective."""
        self.set_sense(sense)

    #
    # Singleton objectives are strange in that we want them to be
    # both be constructed but have len() == 0 when not initialized with
    # anything (at least according to the unit tests that are
    # currently in place). So during initialization only, we will
    # treat them as "indexed" objects where things like
    # Objective.Skip are managed. But after that they will behave
    # like _ObjectiveData objects where set_value does not handle
    # Objective.Skip but expects a valid expression or None
    #

    def clear(self):
        self._data = {}

    def set_value(self, expr):
        """Set the expression of this objective."""
        if not self._constructed:
            raise ValueError(
                "Setting the value of objective '%s' "
                "before the Objective has been constructed (there "
                "is currently no object to set)." % (self.name)
            )
        if not self._data:
            self._data[None] = self
        return super().set_value(expr)

    def set_sense(self, sense):
        """Set the sense (direction) of this objective."""
        if self._constructed:
            if len(self._data) == 0:
                self._data[None] = self
            return _GeneralObjectiveData.set_sense(self, sense)
        raise ValueError(
            "Setting the sense of objective '%s' "
            "before the Objective has been constructed (there "
            "is currently no object to set)." % (self.name)
        )

    #
    # Leaving this method for backward compatibility reasons.
    # (probably should be removed)
    #
    def add(self, index, expr):
        """Add an expression with a given index."""
        if index is not None:
            raise ValueError(
                "ScalarObjective object '%s' does not accept "
                "index values other than None. Invalid value: %s" % (self.name, index)
            )
        self.set_value(expr)
        return self


class IndexedObjective(Objective):

    #
    # Leaving this method for backward compatibility reasons
    #
    # Note: Beginning after Pyomo 5.2 this method will now validate that
    # the index is in the underlying index set (through 5.2 the index
    # was not checked).
    #
    def add(self, index, expr):
        """Add an objective with a given index."""
        return self.__setitem__(index, expr)