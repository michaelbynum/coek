#pragma once

#include "coek/api/constraint.hpp"
#include "coek/compact/indexed_container.hpp"

namespace coek {

//
// This wrapper class allows the use of forward references to
// ConstraintMap in the declaration of model classes.
//
class ConstraintMap : public IndexedComponent_Map<Constraint>
{
public:

    explicit ConstraintMap(size_t n) 
                : IndexedComponent_Map<Constraint>(n) {}

    explicit ConstraintMap(const std::vector<size_t>& _shape) 
                : IndexedComponent_Map<Constraint>(_shape) {}

    explicit ConstraintMap(const std::initializer_list<size_t>& _shape)
                : IndexedComponent_Map<Constraint>(_shape) {}

    explicit ConstraintMap(ConcreteSet& arg)
                : IndexedComponent_Map<Constraint>(arg) {}

    ConstraintMap& name(const std::string& str)
        {
        repn->_name = str;
        return *this;
        }

};


inline ConstraintMap constraint(size_t n)
{ return ConstraintMap(n); }
inline ConstraintMap constraint(const std::vector<size_t>& shape)
{return ConstraintMap(shape); }
inline ConstraintMap constraint(const std::initializer_list<size_t>& shape)
{return ConstraintMap(shape); }

inline ConstraintMap constraint(const std::string& name, size_t n)
{ return constraint(n).name(name); }
inline ConstraintMap constraint(const std::string& name, const std::vector<size_t>& shape)
{return constraint(shape).name(name); }
inline ConstraintMap constraint(const std::string& name, const std::initializer_list<size_t>& shape)
{ return constraint(shape).name(name); }

}