#pragma once

#include "model.hpp"
#include "model/simple.hpp"


class Simple_ADModel : public Simple_ExprModel, public ADModel
{
public:

    Simple_ExprModel* expr_model;
    std::vector<Variable*> variables;
    typedef std::set<Variable*, bool(*)(const Variable*, const Variable*)>::iterator variables_iterator_type;
    std::vector<std::vector<int> > J_rc;
    std::vector<std::vector<Variable*> > J;
    std::vector< std::list<NumericValue*> > builds_f;

    void build();

    int num_variables()
        { return variables.size(); }

    void set_variables(std::vector<double>& x);

    void set_variables(const double* x, int n);

    void print(std::ostream& ostr);


    double compute_f(unsigned int i);

    void compute_df(double& f, std::vector<double>& df, unsigned int i);

    void compute_c(std::vector<double>& c);

    void compute_dc(std::vector<double>& dc, unsigned int i);

    void compute_adjoints(unsigned int i);

    void compute_Hv(std::vector<double>& v, std::vector<double>& Hv, unsigned int i);

};

template <>
inline void initialize_admodel(Simple_ADModel& admodel, Simple_ExprModel& model)
{
admodel.expr_model = &model;
admodel.build();
}

