#include "expr/ast_visitors.hpp"
#include "coek_model.hpp"
#include "solvers/testsolver.hpp"

#ifdef WITH_IPOPT
#include "solvers/ipopt.hpp"
#endif

#ifdef WITH_GUROBI
#include "solvers/coek_gurobi.hpp"
#endif


namespace coek {


void SolverCache::find_updated_values()
{
vupdates.clear();
pupdates.clear();

for (auto it=vcache.begin(); it != vcache.end(); ++it) {
    if (fabs(it->second - it->first->value) > tolerance) {
        vupdates.insert(it->first);
        it->second = it->first->value;
        }
    }
for (auto it=pcache.begin(); it != pcache.end(); ++it) {
    if (fabs(it->second - it->first->value) > tolerance) {
        pupdates.insert(it->first);
        it->second = it->first->value;
        }
    }

#ifdef DEBUG
std::cout << "Updated Fixed Variables: " << vupdates.size() << std::endl;
std::cout << "Updated Parameters:      " << pupdates.size() << std::endl;
#endif
}


SolverRepn* create_solver(std::string& name)
{
if (name == "test")
    return new TestSolver();

#ifdef WITH_GUROBI
if (name == "gurobi")
    return new GurobiSolver();
#endif

return 0;
}


NLPSolverRepn* create_nlpsolver(std::string& name)
{

#ifdef WITH_IPOPT
if (name == "ipopt")
    return new IpoptSolver();
#endif

return 0;
}


template <typename K, typename V>
V& GetWithDef(std::unordered_map <K,V> & m, const K & key)
{
typename std::unordered_map<K,V>::iterator it = m.find( key );
if ( it == m.end() ) {
    V tmp;
    m[key] = tmp;
    return m[key];
    }
else {
    return it->second;
    }
}


void SolverRepn::load(Model& _model)
{
#if 0
    std::map<VariableTerm*, double> vcache;
    std::map<ParameterTerm*, double> pcache;
#endif
vcache.clear();
pcache.clear();

vconstvals.clear();
pconstvals.clear();
vlinvals.clear();
plinvals.clear();
vquadvals.clear();
pquadvals.clear();
vnonlvals.clear();
pnonlvals.clear();

repn.resize(_model.objectives.size() + _model.constraints.size());

model = &_model;

// Collect mutable repn data
{
size_t j=0;
for (size_t i=0; i<_model.objectives.size(); i++, j++)
    repn[j].collect_terms( _model.objectives[i] );
for (size_t i=0; i<_model.constraints.size(); i++, j++)
    repn[j].collect_terms( _model.constraints[i] );
}

// Populate the maps that identify mutable model values
int nmutable=0;
for (size_t j=0; j<repn.size(); j++) {
    MutableNLPExpr& _repn = repn[j];
    if (! _repn.is_mutable())
        continue;

    nmutable++;
    std::unordered_set<VariableTerm*> fixed_vars;    
    std::unordered_set<ParameterTerm*> params;    
    
    mutable_values(_repn.constval.repn, fixed_vars, params);
    for (auto it=fixed_vars.begin(); it != fixed_vars.end(); ++it)
        GetWithDef(vconstvals, *it).insert(j);
    for (auto it=params.begin(); it != params.end(); ++it)
        GetWithDef(pconstvals, *it).insert(j);

    for (size_t i=0; i<_repn.linear_coefs.size(); i++) {
        fixed_vars.clear();
        params.clear();
        mutable_values(_repn.linear_coefs[i].repn, fixed_vars, params);
        for (auto it=fixed_vars.begin(); it != fixed_vars.end(); ++it)
            GetWithDef(vlinvals, *it).insert(std::pair<size_t,size_t>(j, i));
        for (auto it=params.begin(); it != params.end(); ++it)
            GetWithDef(plinvals, *it).insert(std::pair<size_t,size_t>(j, i));
        }

    for (size_t i=0; i<_repn.quadratic_coefs.size(); i++) {
        fixed_vars.clear();
        params.clear();
        mutable_values(_repn.quadratic_coefs[i].repn, fixed_vars, params);
        for (auto it=fixed_vars.begin(); it != fixed_vars.end(); ++it)
            GetWithDef(vquadvals, *it).insert(std::pair<size_t,size_t>(j, i));
        for (auto it=params.begin(); it != params.end(); ++it)
            GetWithDef(pquadvals, *it).insert(std::pair<size_t,size_t>(j, i));
        }

    fixed_vars.clear();
    params.clear();
    mutable_values(_repn.nonlinear.repn, fixed_vars, params);
    for (auto it=fixed_vars.begin(); it != fixed_vars.end(); ++it)
        GetWithDef(vnonlvals, *it).insert(j);
    for (auto it=params.begin(); it != params.end(); ++it)
        GetWithDef(pnonlvals, *it).insert(j);
    }

#ifdef DEBUG
std::cout << "# Model Expressions:   " << repn.size() << std::endl;
std::cout << "# Mutable Expressions: " << nmutable << std::endl;
#endif
}


bool SolverRepn::initial_solve()
{
if (initial) {
    for (auto it=vconstvals.begin(); it != vconstvals.end(); ++it)
        vcache[it->first] = it->first->value;
    for (auto it=pconstvals.begin(); it != pconstvals.end(); ++it)
        pcache[it->first] = it->first->value;

    for (auto it=vlinvals.begin(); it != vlinvals.end(); ++it)
        vcache[it->first] = it->first->value;
    for (auto it=plinvals.begin(); it != plinvals.end(); ++it)
        pcache[it->first] = it->first->value;

    for (auto it=vquadvals.begin(); it != vquadvals.end(); ++it)
        vcache[it->first] = it->first->value;
    for (auto it=pquadvals.begin(); it != pquadvals.end(); ++it)
        pcache[it->first] = it->first->value;

    for (auto it=vnonlvals.begin(); it != vnonlvals.end(); ++it)
        vcache[it->first] = it->first->value;
    for (auto it=pnonlvals.begin(); it != pnonlvals.end(); ++it)
        pcache[it->first] = it->first->value;

    vupdates.clear();
    pupdates.clear();

    initial=false;
    return true;
    }
else {
    find_updated_values();
    find_updated_coefs();
    return false;
    }
}


void SolverRepn::find_updated_coefs()
{
updated_coefs.clear();

for (auto it=vupdates.begin(); it != vupdates.end(); ++it) {
    try {
        std::set<size_t>& expr = vconstvals[*it];
        for (auto jt=expr.begin(); jt != expr.end(); ++jt)
            updated_coefs.insert( std::tuple<size_t, size_t, size_t>(*jt, 0, 0) );
        }
    catch (...) {
        // TODO
        }
    try {
        std::set<std::tuple<size_t,size_t> >& expr = vlinvals[*it];
        for (auto jt=expr.begin(); jt != expr.end(); ++jt)
            updated_coefs.insert( std::tuple<size_t, size_t, size_t>(std::get<0>(*jt), 1, std::get<1>(*jt)) );
        }
    catch (...) {
        // TODO
        }
    try {
        std::set<std::tuple<size_t,size_t> >& expr = vquadvals[*it];
        for (auto jt=expr.begin(); jt != expr.end(); ++jt)
            updated_coefs.insert( std::tuple<size_t, size_t, size_t>(std::get<0>(*jt), 2, std::get<1>(*jt)) );
        }
    catch (...) {
        // TODO
        }
    try {
        std::set<size_t>& expr = vnonlvals[*it];
        for (auto jt=expr.begin(); jt != expr.end(); ++jt)
            updated_coefs.insert( std::tuple<size_t, size_t, size_t>(*jt, 3, 0) );
        }
    catch (...) {
        // TODO
        }
    }

for (auto it=pupdates.begin(); it != pupdates.end(); ++it) {
    try {
        std::set<size_t>& expr = pconstvals[*it];
        for (auto jt=expr.begin(); jt != expr.end(); ++jt)
            updated_coefs.insert( std::tuple<size_t, size_t, size_t>(*jt, 0, 0) );
        }
    catch (...) {
        // TODO
        }
    try {
        std::set<std::tuple<size_t,size_t> >& expr = plinvals[*it];
        for (auto jt=expr.begin(); jt != expr.end(); ++jt)
            updated_coefs.insert( std::tuple<size_t, size_t, size_t>(std::get<0>(*jt), 1, std::get<1>(*jt)) );
        }
    catch (...) {
        // TODO
        }
    try {
        std::set<std::tuple<size_t,size_t> >& expr = pquadvals[*it];
        for (auto jt=expr.begin(); jt != expr.end(); ++jt)
            updated_coefs.insert( std::tuple<size_t, size_t, size_t>(std::get<0>(*jt), 2, std::get<1>(*jt)) );
        }
    catch (...) {
        // TODO
        }
    try {
        std::set<size_t>& expr = pnonlvals[*it];
        for (auto jt=expr.begin(); jt != expr.end(); ++jt)
            updated_coefs.insert( std::tuple<size_t, size_t, size_t>(*jt, 3, 0) );
        }
    catch (...) {
        // TODO
        }
    }

#ifdef DEBUG
std::cout << "Updated Coefficients:    " << updated_coefs.size() << std::endl;
#endif
}


bool SolverCache::get_option(int option, int& value) const
{ throw std::runtime_error("Solver does not support get_option with integer name and integer value");}
bool SolverCache::get_option(int option, double& value) const
{ throw std::runtime_error("Solver does not support get_option with integer name and double value");}
bool SolverCache::get_option(int option, std::string& value) const
{ throw std::runtime_error("Solver does not support get_option with integer name and double ngvalue");}

void SolverCache::set_option(int option, int value)
{ throw std::runtime_error("Solver does not support set_option with integer name and integer value");}
void SolverCache::set_option(int option, double value)
{ throw std::runtime_error("Solver does not support set_option with integer name and double value");}
void SolverCache::set_option(int option, const std::string value)
{ throw std::runtime_error("Solver does not support set_option with integer name and double ngvalue");}

bool SolverCache::get_option(const std::string& option, int& value) const
{
auto curr = integer_options.find(option);
if (curr == integer_options.end())
    return false;
value = curr->second;
return true;
}

bool SolverCache::get_option(const std::string& option, double& value) const
{
auto curr = double_options.find(option);
if (curr == double_options.end())
    return false;
value = curr->second;
return true;
}

bool SolverCache::get_option(const std::string& option, std::string& value) const
{
auto curr = string_options.find(option);
if (curr == string_options.end())
    return false;
value = curr->second;
return true;
}

void SolverCache::set_option(const std::string& option, int value)
{
integer_options[option] = value;
}

void SolverCache::set_option(const std::string& option, double value)
{
double_options[option] = value;
}

void SolverCache::set_option(const std::string& option, const std::string value)
{
string_options[option] = value;
}


}
