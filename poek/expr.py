from _expr.lib import *


class NumericValue(object):

    __slots__ = ()

    # This is required because we define __eq__
    __hash__ = None

    def size(self):
        return expr_size(self.ptr)

    def __lt__(self,other):
        """
        Less than operator

        This method is called when Python processes statements of the form::
        
            self < other
            other > self
        """
        return _generate_relational_expression(_lt, self, other)

    def __gt__(self,other):
        """
        Greater than operator

        This method is called when Python processes statements of the form::
        
            self > other
            other < self
        """
        return _generate_relational_expression(_lt, other, self)

    def __le__(self,other):
        """
        Less than or equal operator

        This method is called when Python processes statements of the form::
        
            self <= other
            other >= self
        """
        return _generate_relational_expression(_le, self, other)

    def __ge__(self,other):
        """
        Greater than or equal operator

        This method is called when Python processes statements of the form::
        
            self >= other
            other <= self
        """
        return _generate_relational_expression(_le, other, self)

    def __eq__(self,other):
        """
        Equal to operator

        This method is called when Python processes the statement::
        
            self == other
        """
        return _generate_relational_expression(_eq, self, other)

    def __add__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            self + other
        """
        if other.__class__ is int:
            return expression( add_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            return expression( add_expr_double(self.ptr, other) )
        else:
            return expression( add_expr_expression(self.ptr, other.ptr) )

    def __mul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::
        
            self * other
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            return expression( mul_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            return expression( mul_expr_double(self.ptr, other) )
        else:
            return expression( mul_expr_expression(self.ptr, other.ptr) )

    def __radd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            other + self
        """
        if other.__class__ is int:
            return expression( radd_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            return expression( radd_expr_double(other, self.ptr) )

    def __rmul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::
        
            other * self

        when other is not a :class:`NumericValue <pyomo.core.expr.numvalue.NumericValue>` object.
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            return expression( rmul_expr_int(other, self.ptr) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            return expression( rmul_expr_double(other, self.ptr) )

    def __iadd__(self,other):
        """
        Binary addition

        This method is called when Python processes the statement::
        
            self += other
        """
        if other.__class__ is int:
            return expression( add_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            return expression( add_expr_double(self.ptr, other) )
        else:
            return expression( add_expr_expression(self.ptr, other.ptr) )

    def __imul__(self,other):
        """
        Binary multiplication

        This method is called when Python processes the statement::

            self *= other
        """
        if other.__class__ is int:
            if other == 0:
                return 0
            return expression( mul_expr_int(self.ptr, other) )
        elif other.__class__ is float:
            if other == 0.0:
                return 0.0
            return expression( mul_expr_double(self.ptr, other) )
        else:
            return expression( mul_expr_expression(self.ptr, other.ptr) )


class parameter(NumericValue):

    __slots__ = ('ptr')

    def __init__(self, value):
        if value.__class__ is int:
            self.ptr = create_parameter_int(value, 0)
        else:
            self.ptr = create_parameter_double(value, 0)


class variable(object):

    def __new__(cls, *args, **kwds):
        if len(args) == 0 or args[0] == 1:
            return variable_single(**kwds)
        return variable_array(args[0], **kwds)
            

class variable_single(NumericValue):

    __slots__ = ('ptr', 'name')

    def __init__(self, name=None):
        self.name = name
        self.ptr = create_variable(0,0)   # TODO: add 'within' argument

    def __str__(self):
        if self.name is None:
            index = get_variable_index(self.ptr)
            self.name = 'x%d' % index
        return self.name


class variable_array(object):

    __slots__ = ('ptrs', 'name', 'num')

    def __init__(self, num, name=None):
        self.num = num 
        self.name = name
        self.ptr = create_variable_array(0,0,num)   # TODO: add 'within' argument


class expression(NumericValue):

    __slots__ = ('ptr',)

    def __init__(self, ptr):
        self.ptr = ptr

    def show(self):
        print_expr(self.ptr)
