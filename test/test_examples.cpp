
#include <iostream>
#include "catch.hpp"
#include "coek/ast/base_terms.hpp"
#include "coek/coek.hpp"

//
// EXAMPLES
//
void rosenbr_example(coek::Model& model);
std::vector<double> rosenbr_soln {1,1};

void simplelp1_example(coek::Model& model);
std::vector<double> simplelp1_soln {375,250};

void invquad_example(coek::Model& model, std::vector<coek::Parameter>& p);
std::vector<double> invquad_soln_5 {-10,-10,-10,-10,-10};


void check(std::vector<coek::Variable>& variables, std::vector<double>& soln)
{
for (size_t i=0; i<variables.size(); i++)
    REQUIRE(variables[i].get_value() == Approx(soln[i]));
}


#ifdef WITH_IPOPT
TEST_CASE( "ipopt", "[smoke]" ) {

  SECTION( "rosenbr" ) {
    coek::Model m;
    rosenbr_example(m);

    coek::NLPModel nlp(m, "cppad");
    coek::NLPSolver solver("ipopt");

    solver.set_option("print_level", 0);
    solver.solve(nlp);

    check(m.repn->variables, rosenbr_soln);
  }

  SECTION( "invquad" ) {
    std::vector<coek::Parameter> p(5);
    for (auto it=p.begin(); it != p.end(); ++it)
        it->set_value(0.5);

    WHEN( "solve" ) {
        coek::Model m;
        invquad_example(m, p);

        coek::NLPModel nlp(m, "cppad");
        coek::NLPSolver solver("ipopt");

        solver.set_option("print_level", 0);
        solver.solve(nlp);

        check(m.repn->variables, invquad_soln_5);
    }
    WHEN( "resolve - Same start" ) {
        coek::Model m;
        invquad_example(m, p);

        coek::NLPModel nlp(m, "cppad");
        coek::NLPSolver solver("ipopt");

        solver.set_option("print_level", 0);
        solver.solve(nlp);

        for (auto it=p.begin(); it != p.end(); ++it)
            it->set_value(-0.5);

        for (size_t i=0; i<nlp.num_variables(); i++)
            nlp.get_variable(i).set_value(0);
        solver.set_option("print_level", 0);
        solver.resolve();

        std::vector<double> invquad_resolve_5 {10,10,10,10,10};
        check(m.repn->variables, invquad_resolve_5);
    }
    WHEN( "resolve - Current point" ) {
        coek::Model m;
        invquad_example(m, p);

        coek::NLPModel nlp(m, "cppad");
        coek::NLPSolver solver("ipopt");

        solver.set_option("print_level", 0);
        solver.solve(nlp);

        for (auto it=p.begin(); it != p.end(); ++it)
            it->set_value(-0.5);

        solver.set_option("print_level", 0);
        solver.resolve();

        check(m.repn->variables, invquad_soln_5);
    }
    WHEN( "resolve - Warm Start" ) {
        coek::Model m;
        invquad_example(m, p);

        coek::NLPModel nlp(m, "cppad");
        coek::NLPSolver solver("ipopt");

        solver.set_option("print_level", 0);
        solver.solve(nlp);

        for (auto it=p.begin(); it != p.end(); ++it)
            it->set_value(-0.5);

        // Even though we set the value of the initial point,
        // the warm starting option should ignore this and 
        // restart the solve from where it ended last time.
        for (size_t i=0; i<nlp.num_variables(); i++)
            nlp.get_variable(i).set_value(0);

        solver.set_option("warm_start_init_point", "yes");
        solver.set_option("print_level", 0);
        solver.resolve();

        check(m.repn->variables, invquad_soln_5);
    }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
#endif


#ifdef WITH_GUROBI
TEST_CASE( "gurobi", "[smoke]" ) {

  SECTION( "simplelp1" ) {
    coek::Model m;
    simplelp1_example(m);

    coek::Solver solver("gurobi");

    solver.set_option("OutputFlag", 0);
    solver.solve(m);

    check(m.repn->variables, simplelp1_soln);
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
#endif