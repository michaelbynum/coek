#pragma once

#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <unordered_map>
#include <map>
#include "coek_expr.hpp"


namespace coek {

class Model;
class NLPModel;

class SolverCache
{
public:

    std::unordered_map<VariableTerm*, double> vcache;
    std::unordered_map<ParameterTerm*, double> pcache;

    double tolerance;
    std::unordered_set<VariableTerm*> vupdates;
    std::unordered_set<ParameterTerm*> pupdates;

    std::map<std::string,std::string> string_options;
    std::map<std::string,int> integer_options;
    std::map<std::string,double> double_options;

public:

    SolverCache(void)
        : tolerance(1e-12) {}

    virtual void find_updated_values();

    // TODO - Move these get/set methods into a separate common SolverRepn base class.
    
    virtual bool get_option(const std::string& option, int& value) const;
    virtual bool get_option(const std::string& option, double& value) const;
    virtual bool get_option(const std::string& option, std::string& value) const;
    virtual bool get_option(int option, int& value) const;
    virtual bool get_option(int option, double& value) const;
    virtual bool get_option(int option, std::string& value) const;

    virtual void set_option(const std::string& option, int value);
    virtual void set_option(const std::string& option, double value);
    virtual void set_option(const std::string& option, const std::string value);
    virtual void set_option(int option, int value);
    virtual void set_option(int option, double value);
    virtual void set_option(int option, const std::string value);
};


//
// The base solver class that defines the API used by Python
//
class SolverRepn : public SolverCache
{
public:

    Model* model;

    std::vector<MutableNLPExpr> repn;

    std::unordered_map<VariableTerm*, std::set<size_t> > vconstvals;
    std::unordered_map<ParameterTerm*, std::set<size_t> > pconstvals;

    std::unordered_map<VariableTerm*, std::set<std::tuple<size_t,size_t> > > vlinvals;
    std::unordered_map<ParameterTerm*, std::set<std::tuple<size_t,size_t> > > plinvals;

    std::unordered_map<VariableTerm*, std::set<std::tuple<size_t,size_t> > > vquadvals;
    std::unordered_map<ParameterTerm*, std::set<std::tuple<size_t,size_t> > > pquadvals;

    std::unordered_map<VariableTerm*, std::set<size_t> > vnonlvals;
    std::unordered_map<ParameterTerm*, std::set<size_t> > pnonlvals;

    std::set<std::tuple<size_t, size_t, size_t> > updated_coefs;

public:
    
    SolverRepn(void) 
        : SolverCache(), model(0), initial(true) {}
    virtual ~SolverRepn() {}

    virtual void load(Model& model);

    virtual int resolve() = 0;

    virtual int solve(Model& model) = 0;

    bool initial_solve();
    void find_updated_coefs();

protected:

    bool initial;
};


SolverRepn* create_solver(std::string& name);


class NLPSolverRepn : public SolverCache
{
public:

    NLPModel* model;

public:

    NLPSolverRepn(void) 
        : SolverCache(), model(0) {}
    virtual ~NLPSolverRepn() {}

    virtual void load(NLPModel& _model)
        {model = &_model;}

    virtual int resolve() = 0;

    virtual int solve(NLPModel& model) = 0;

    virtual bool initial_solve()
        {return false;}

protected:

    bool initial;
};

NLPSolverRepn* create_nlpsolver(std::string& name);

}