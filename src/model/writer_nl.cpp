#include <map>
#include "expr/ast_visitors.hpp"
#include "coek_model.hpp"

namespace coek {

namespace {

void format(std::ostream& ostr, double value)
{
// TODO - format string
ostr << value;
}



class PrintExpr : public Visitor
{
public:

    std::ostream& ostr;
    std::map<int,int>& varmap;

public:

    PrintExpr(std::ostream& _ostr, std::map<int,int>& _varmap)
        : ostr(_ostr), varmap(_varmap) {}

    void visit(ConstantTerm& arg);
    void visit(ParameterTerm& arg);
    void visit(VariableTerm& arg);
    void visit(MonomialTerm& arg);
    void visit(InequalityTerm& arg);
    void visit(EqualityTerm& arg);
    void visit(NegateTerm& arg);
    void visit(PlusTerm& arg);
    void visit(TimesTerm& arg);
    void visit(DivideTerm& arg);
    void visit(AbsTerm& arg);
    void visit(CeilTerm& arg);
    void visit(FloorTerm& arg);
    void visit(ExpTerm& arg);
    void visit(LogTerm& arg);
    void visit(Log10Term& arg);
    void visit(SqrtTerm& arg);
    void visit(SinTerm& arg);
    void visit(CosTerm& arg);
    void visit(TanTerm& arg);
    void visit(SinhTerm& arg);
    void visit(CoshTerm& arg);
    void visit(TanhTerm& arg);
    void visit(ASinTerm& arg);
    void visit(ACosTerm& arg);
    void visit(ATanTerm& arg);
    void visit(ASinhTerm& arg);
    void visit(ACoshTerm& arg);
    void visit(ATanhTerm& arg);
    void visit(PowTerm& arg);
};


void PrintExpr::visit(ConstantTerm& arg)
{ ostr << "n" << arg.value << std::endl; }

void PrintExpr::visit(ParameterTerm& arg)
{ ostr << "n" << arg.value << std::endl; }

void PrintExpr::visit(VariableTerm& arg)
{ ostr << "v" << varmap[arg.index] << std::endl; }

void PrintExpr::visit(MonomialTerm& arg)
{
ostr << "o2" << std::endl;
ostr << "n" << arg.coef << std::endl;
ostr << "v" << varmap[arg.var->index] << std::endl;
}

void PrintExpr::visit(InequalityTerm& arg)
{ arg.body->accept(*this); }

void PrintExpr::visit(EqualityTerm& arg)
{ arg.body->accept(*this); }

void PrintExpr::visit(NegateTerm& arg)
{
ostr << "o16" << std::endl;
arg.body->accept(*this);
}

void PrintExpr::visit(PlusTerm& arg)
{
if (arg.n == 2)
    ostr << "o0" << std::endl;
else
    ostr << "o54" << std::endl;
std::vector<expr_pointer_t>& vec = *(arg.data);
for (size_t i=0; i<arg.n; i++)
    vec[i]->accept(*this);
}

void PrintExpr::visit(TimesTerm& arg)
{
ostr << "o2" << std::endl;
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}

void PrintExpr::visit(DivideTerm& arg)
{
ostr << "o3" << std::endl;
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}

#define PrintExpr_FN(FN, TERM)\
void PrintExpr::visit(TERM& arg)\
{\
ostr << #FN << std::endl;\
arg.body->accept(*this);\
}

PrintExpr_FN(o15, AbsTerm)
PrintExpr_FN(o14, CeilTerm)
PrintExpr_FN(o13, FloorTerm)
PrintExpr_FN(o44, ExpTerm)
PrintExpr_FN(o43, LogTerm)
PrintExpr_FN(o42, Log10Term)
PrintExpr_FN(o39, SqrtTerm)
PrintExpr_FN(o41, SinTerm)
PrintExpr_FN(o46, CosTerm)
PrintExpr_FN(o38, TanTerm)
PrintExpr_FN(o40, SinhTerm)
PrintExpr_FN(o45, CoshTerm)
PrintExpr_FN(o37, TanhTerm)
PrintExpr_FN(o51, ASinTerm)
PrintExpr_FN(o53, ACosTerm)
PrintExpr_FN(o49, ATanTerm)
PrintExpr_FN(o50, ASinhTerm)
PrintExpr_FN(o52, ACoshTerm)
PrintExpr_FN(o47, ATanhTerm)

void PrintExpr::visit(PowTerm& arg)
{
ostr << "o5" << std::endl;\
arg.lhs->accept(*this);
arg.rhs->accept(*this);
}


void print_expr(std::ostream& ostr, MutableNLPExpr& repn, std::map<int,int>& varmap)
{
bool nonlinear = not repn.nonlinear.is_constant();
bool quadratic = repn.quadratic_coefs.size() > 0;
if (nonlinear and quadratic)
    ostr << "o0" << std::endl;
if (quadratic) {
    std::map<std::pair<int,int>,double> term;
    for (size_t i=0; i<repn.quadratic_coefs.size(); i++) {
        int lhs = varmap[repn.quadratic_lvars[i]->index];
        int rhs = varmap[repn.quadratic_rvars[i]->index];
        if (rhs < lhs)
            std::swap(lhs,rhs);
        auto key = std::pair<int,int>(lhs, rhs);
        if (term.find(key) == term.end())
            term[key] = repn.quadratic_coefs[i].get_value();
        else
            term[key] += repn.quadratic_coefs[i].get_value();
        }
    if (term.size() == 2)
        ostr << "o0" << std::endl;
    else if (term.size() > 2)
        ostr << "o54" << std::endl;
    for (auto it=term.begin(); it != term.end(); it++) {
        double coef = it->second;
        if (coef != 1) {
            ostr << "o2" << std::endl;
            ostr << "n" << coef << std::endl;
            }
        ostr << "o2" << std::endl;
        ostr << "v" << it->first.first << std::endl;
        ostr << "v" << it->first.second << std::endl;
        }
    }
if (nonlinear) {
    PrintExpr visitor(ostr, varmap);
    repn.nonlinear.repn->accept(visitor);
    }
}

}



void write_nl_problem(Model& model, std::ostream& ostr)
{
if (model.objectives.size() == 0) {
    std::cerr << "Error writing NL file: No objectives specified!" << std::endl;
    return;
    }
if (model.objectives.size() > 1) {
    std::cerr << "Error writing NL file: More than one objective defined!" << std::endl;
    return;
    }

//
// Process Model to Create NL Header
//
std::map<int,Variable> varobj;
std::set<int> vars;
std::set<int> nonlinear_vars_obj;
std::set<int> nonlinear_vars_con;
int num_inequalities=0;
int num_equalities=0;
int nonl_objectives=0;
int nonl_constraints=0;

std::set<int> linear_vars;
int num_linear_binary_vars=0;
int num_linear_integer_vars=0;
int num_nonlinear_obj_int_vars=0;
int num_nonlinear_con_int_vars=0;
int num_nonlinear_both_int_vars=0;

int nnz_Jacobian=0;
int nnz_gradient=0;

// Objectives
std::vector<MutableNLPExpr> o_expr(model.objectives.size());
int ctr=0;
for (auto it=model.objectives.begin(); it != model.objectives.end(); ++it, ctr++) {
    o_expr[ctr].collect_terms(*it);
    if ((o_expr[ctr].quadratic_coefs.size() > 0) or (not o_expr[ctr].nonlinear.is_constant()))
        nonl_objectives++;
    for (auto it=o_expr[ctr].linear_vars.begin(); it != o_expr[ctr].linear_vars.end(); it++) {
        auto var = *it;
        linear_vars.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        }
    for (auto it=o_expr[ctr].quadratic_lvars.begin(); it != o_expr[ctr].quadratic_lvars.end(); it++) {
        auto var = *it;
        nonlinear_vars_obj.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        }
    for (auto it=o_expr[ctr].quadratic_rvars.begin(); it != o_expr[ctr].quadratic_rvars.end(); it++) {
        auto var = *it;
        nonlinear_vars_obj.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        }
    for (auto it=o_expr[ctr].nonlinear_vars.begin(); it != o_expr[ctr].nonlinear_vars.end(); it++) {
        auto var = *it;
        nonlinear_vars_obj.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        }
    }

// Since we have just one objective, the # of variables is the # of nonzeros in gradients
nnz_gradient=vars.size();

// Constraints
std::vector<MutableNLPExpr> c_expr(model.constraints.size());
ctr=0;
for (std::vector<Constraint>::iterator it=model.constraints.begin(); it != model.constraints.end(); ++it, ctr++) {
    c_expr[ctr].collect_terms(*it);
    if (it->is_inequality())
        num_inequalities++;
    else
        num_equalities++;
    if ((c_expr[ctr].quadratic_coefs.size() > 0) or (not c_expr[ctr].nonlinear.is_constant()))
        nonl_constraints++;

    std::set<int> curr_vars;

    for (auto it=c_expr[ctr].linear_vars.begin(); it != c_expr[ctr].linear_vars.end(); it++) {
        auto var = *it;
        linear_vars.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        curr_vars.insert(var->index);
        }
    for (auto it=c_expr[ctr].quadratic_lvars.begin(); it != c_expr[ctr].quadratic_lvars.end(); it++) {
        auto var = *it;
        nonlinear_vars_con.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        curr_vars.insert(var->index);
        }
    for (auto it=c_expr[ctr].quadratic_rvars.begin(); it != c_expr[ctr].quadratic_rvars.end(); it++) {
        auto var = *it;
        nonlinear_vars_con.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        curr_vars.insert(var->index);
        }
    for (auto it=c_expr[ctr].nonlinear_vars.begin(); it != c_expr[ctr].nonlinear_vars.end(); it++) {
        auto var = *it;
        nonlinear_vars_con.insert(var->index);
        vars.insert(var->index);
        varobj[var->index] = var;
        curr_vars.insert(var->index);
        }

    // Add Jacobian terms for each constraint
    nnz_Jacobian += curr_vars.size();
    }

for (auto it=linear_vars.begin(); it != linear_vars.end(); it++) {
    auto& var = varobj[*it];
    if (var.is_binary())
        num_linear_binary_vars++;
    else if (var.is_integer())
        num_linear_integer_vars++;
    }

int nonlinear_vars_both=0;
for (auto it=nonlinear_vars_obj.begin(); it != nonlinear_vars_obj.end(); it++) {
    bool flag = varobj[*it].is_binary() or varobj[*it].is_integer();
    if (flag)
       num_nonlinear_obj_int_vars++;
    if (nonlinear_vars_con.find(*it) != nonlinear_vars_con.end()) {
        nonlinear_vars_both++;
        if (flag)
           num_nonlinear_both_int_vars++;
        }
    }
for (auto it=nonlinear_vars_con.begin(); it != nonlinear_vars_con.end(); it++) {
    if (varobj[*it].is_binary() or varobj[*it].is_integer())
       num_nonlinear_con_int_vars++;
    }

// Map Variable index to NL variable ID (0 ... n_vars-1)
std::map<int,int> varmap;
ctr = 0;
for (auto it=vars.begin(); it != vars.end(); it++)
    varmap[*it] = ctr++;

// Compute linear Jacobian and Gradient values
std::vector<std::set<int>> k_count(vars.size());
std::vector<std::map<int,double>> G(o_expr.size());
std::vector<std::map<int,double>> J(c_expr.size());

ctr=0;
for (auto it=o_expr.begin(); it != o_expr.end(); ++it, ctr++) {
    for (auto jt=it->quadratic_lvars.begin(); jt!= it->quadratic_lvars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (auto jt=it->quadratic_rvars.begin(); jt!= it->quadratic_rvars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (auto jt=it->nonlinear_vars.begin(); jt!= it->nonlinear_vars.end(); ++jt) {
        G[ctr][ varmap[(*jt)->index] ] = 0;
        }
    for (size_t j=0; j<it->linear_coefs.size(); j++) {
        auto index = varmap[it->linear_vars[j]->index];
        if (G[ctr].find(index) == G[ctr].end())
            G[ctr][index] = it->linear_coefs[j].get_value();
        else
            G[ctr][index] += it->linear_coefs[j].get_value();
        }
    }
ctr=0;
for (auto it=c_expr.begin(); it != c_expr.end(); ++it, ctr++) {
    for (auto jt=it->quadratic_lvars.begin(); jt!= it->quadratic_lvars.end(); ++jt) {
        int index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (auto jt=it->quadratic_rvars.begin(); jt!= it->quadratic_rvars.end(); ++jt) {
        int index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (auto jt=it->nonlinear_vars.begin(); jt!= it->nonlinear_vars.end(); ++jt) {
        int index = varmap[(*jt)->index];
        k_count[ index ].insert(ctr);
        J[ctr][ index ] = 0;
        }
    for (size_t j=0; j<it->linear_coefs.size(); j++) {
        int index = varmap[it->linear_vars[j]->index];
        if (J[ctr].find(index) == J[ctr].end()) {
            k_count[ index ].insert(ctr);
            J[ctr][index] = it->linear_coefs[j].get_value();
            }
        else
            J[ctr][index] += it->linear_coefs[j].get_value();
        }
    }

//
// Write NL Header
//
// This API seems poorly documented.  Is the 2005 paper the defining reference?  Pyomo writes a header that doesn't conform to it...
//
ostr << "g3 1 1 0 # unnamed problem generated by COEK" << std::endl;
ostr << " " << vars.size() << " " << (num_inequalities+num_equalities) << " 1 0 " << num_equalities << " 0 # vars, constraints, objectives, ranges, eqns, lcons" << std::endl;
ostr << " " << nonl_constraints << " " << nonl_objectives << " # nonlinear constraints, objectives" << std::endl;
ostr << " 0 0 # network constraints: nonlinear, linear" << std::endl;
ostr << " " << nonlinear_vars_con.size() << " " << nonlinear_vars_obj.size() << " " << nonlinear_vars_both << " # nonlinear vars in constraints, objectives, both" << std::endl;
ostr << " 0 0 0 1 # linear network variables; functions; arith, flags" << std::endl;
ostr << " " << num_linear_binary_vars << " " << num_linear_integer_vars << " " << num_nonlinear_both_int_vars << " " << num_nonlinear_con_int_vars << " " << num_nonlinear_obj_int_vars << " # discrete variables: binary, integer, nonlinear (b,c,o)" << std::endl;
ostr << " " << nnz_Jacobian << " " << nnz_gradient << " # nonzeros in Jacobian, gradients" << std::endl;
ostr << " 0 0 # max name lengths: constraints, variables" << std::endl;
ostr << " 0 0 0 0 0 # common exprs: b,c,o,c1,o1" << std::endl;

//
// "C" section - nonlinear constraint segments
//
ctr = 0;
for (auto it=c_expr.begin(); it != c_expr.end(); it++, ctr++) {
    //std::cout << "HERE " << it->nonlinear.is_constant() << " " << it->quadratic_coefs.size() << std::endl;
    //std::cout << it->nonlinear.to_list() << std::endl;
    if ((not it->nonlinear.is_constant()) or (it->quadratic_coefs.size() > 0)) {
        ostr << "C" << ctr << std::endl;
        print_expr(ostr, *it, varmap);
        }
    else {
        ostr << "C" << ctr << std::endl;
        ostr << "n0" << std::endl;
        }
    }

//
// "O" section - nonlinear objective segments
//
ctr=0;
for (auto it=o_expr.begin(); it != o_expr.end(); ++it, ctr++) {
    if ((not it->nonlinear.is_constant()) or (it->quadratic_coefs.size() > 0)) {
        if (model.sense[ctr] == Model::minimize)
            ostr << "O" << ctr << " 0" << std::endl;
        else
            ostr << "O" << ctr << " 1" << std::endl;
        print_expr(ostr, *it, varmap);
        }
    else {
        if (model.sense[ctr] == Model::minimize)
            ostr << "O" << ctr << " 0" << std::endl;
        else
            ostr << "O" << ctr << " 1" << std::endl;
        ostr << "n0" << std::endl;
        }
    }

//
// "x" section - primal initial values
//
{
auto _it = vars.begin();
if (not std::isnan(varobj[*_it].get_initial())) {
    ostr << "x" << vars.size() << std::endl;
    ctr = 0;
    for (auto it=vars.begin(); it != vars.end(); it++, ctr++) {
        auto tmp = varobj[*it].get_initial();
        if (not std::isnan(tmp))
            ostr << ctr << " " << tmp << std::endl;
        }
    }
}

//
// "r" section - bounds on constraints
//

ostr << "r" << std::endl;
ctr = 0;
for (auto it=model.constraints.begin(); it != model.constraints.end(); ++it, ctr++) {
    if (it->is_inequality()) {
        ostr << "1 ";
        }
    else {
        ostr << "4 ";
        }
    double constval=c_expr[ctr].constval.get_value();
    if (constval == 0)
        format(ostr, constval);
    else
        format(ostr, -1 * constval);
    ostr << std::endl;
    }

//
// "b" section - bounds on variables
//
ostr << "b" << std::endl;
for (auto it=vars.begin(); it != vars.end(); it++) {
    auto var = varobj[*it];
    double lb = var.get_lb();
    double ub = var.get_ub();
    if (lb == -COEK_INFINITY) {
        if (ub == COEK_INFINITY) {
            ostr << "3" << std::endl;
            }
        else {
            ostr << "1 ";
            format(ostr, ub);
            ostr << std::endl;
            }
        }
    else {
        if (ub == COEK_INFINITY) {
            ostr << "2 ";
            format(ostr, lb);
            ostr << std::endl;
            }
        else {
            ostr << "0 ";
            format(ostr, lb);
            ostr << " ";
            format(ostr, ub);
            ostr << std::endl;
            }
        }
    }

//
// "k" section - Jacobian column counts
//
ostr << "k" << (k_count.size()-1) << std::endl;
ctr = 0;
for (size_t i=0; i<(k_count.size()-1); i++) {
    ctr += k_count[i].size();
    ostr << ctr << std::endl;
    }

//
// "J" section - Jacobian sparsity, linear terms
//
for (size_t i=0; i<J.size(); i++) {
    if (J[i].size() == 0) continue;
    ostr << "J" << i << " " << J[i].size() << std::endl;
    for (auto it=J[i].begin(); it!=J[i].end(); ++it) {
        ostr << it->first << " " << it->second << std::endl;
        }
    }

//
// "G" section - Gradient sparsity, linear terms
//
for (size_t i=0; i<G.size(); i++) {
    if (G[i].size() == 0) continue;
    ostr << "G" << i << " " << G[i].size() << std::endl;
    for (auto it=G[i].begin(); it!=G[i].end(); ++it) {
        ostr << it->first << " " << it->second << std::endl;
        }
    }


// DONE
}

}