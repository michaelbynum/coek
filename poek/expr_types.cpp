
#include "expr_types.h"

TypedParameter<int> ZeroParameter(0,false);
TypedParameter<int> OneParameter(1,false);


int Variable::nvariables = 0;
double Variable::_nan = nan(0);

bool variable_comparator(const Variable* lhs, const Variable* rhs)
{
return lhs->index <= rhs->index;
}
