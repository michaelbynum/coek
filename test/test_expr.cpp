
#include <cmath>
#include <memory>
#include <sstream>
#include <iostream>
#include "catch2/catch.hpp"

#include "coek/ast/base_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/coek.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);


TEST_CASE( "model_param", "[smoke]" ) {

  SECTION( "values" ) {
      WHEN( "parameter - 3" ) {
        auto q = coek::parameter("q").value(2);
        REQUIRE( q.value() == 2 );
        q.value(3);
        REQUIRE( q.value() == 3 );
      }
  }

  SECTION( "constructors" ) {
      WHEN( "copy" ) {
        // Simple constructor
        auto q = coek::parameter().value(3);

        // Test copy constructor
        coek::Parameter Q(q);
        REQUIRE( Q.value() == 3 );
      }

      WHEN( "equal" ) {
        // Simple constructor
        auto q = coek::parameter().value(4);

        // Test copy constructor
        auto Q = coek::parameter().value(5);
        Q = q;
        REQUIRE( Q.value() == 4 );
      }
  }

  SECTION( "constructors" ) {
    auto q = coek::parameter("q").value(3);
    REQUIRE( q.name() == "q" );
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    auto q = coek::parameter("q").value(3);
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "model_indexparam", "[smoke]" ) {

  SECTION( "values" ) {
      WHEN( "IndexParameter - 3" ) {
        auto q = coek::set_index("q");
        q.value(1);
        int tmp = -1;
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 1 );
        q.value(3.5);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
      WHEN( "IndexParameter - 3.5" ) {
        auto q = coek::set_index();
        q.value(3.5);
        double tmp = -3.5;
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 3.5 );
        q.value(3);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
      WHEN( "IndexParameter - 'here'" ) {
        auto q = coek::set_index();
        q.value("here");
        std::string tmp = "there";
        auto success = q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == "here" );
        q.value(3.5);
        success = q.get_value(tmp);
        REQUIRE( success == false );
      }
  }

  SECTION( "constructors" ) {
      WHEN( "copy" ) {
        // Simple constructor
        auto q = coek::set_index();
        q.value(3);

        // Test copy constructor
        coek::IndexParameter Q(q);
        int tmp=-1;
        auto success = Q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 3 );
      }

      WHEN( "equal" ) {
        // Simple constructor
        auto q = coek::set_index();
        q.value(4);

        // Test copy constructor
        auto Q = coek::set_index();
        Q.value(5);
        Q = q;
        int tmp=-1;
        auto success = Q.get_value(tmp);
        REQUIRE( success == true );
        REQUIRE( tmp == 4 );
      }
  }

  SECTION( "constructors" ) {
    auto q = coek::set_index("q");
    REQUIRE( q.name() == "q" );
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    auto q = coek::set_index("q");
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "model_variable", "[smoke]" ) {

  SECTION( "constructors" ) {
    WHEN( "simple" ) {
        auto a = coek::variable().lower(0).upper(1).value(2);
        REQUIRE( a.value() == 2 );
        REQUIRE( a.lower() == 0 );
        REQUIRE( a.upper() == 1 );
        // Keep this test?  The integer index depends on whether this test is run first.
        REQUIRE( a.name() == "x(0)" );
    }

    WHEN( "named" ) {
        auto a = coek::variable("test").lower(0).upper(1).value(2);
        REQUIRE( a.value() == 2 );
        REQUIRE( a.lower() == 0 );
        REQUIRE( a.upper() == 1 );
        REQUIRE( a.name() == "test" );
    }

    WHEN( "simple_with_expr_values" ) {
        auto p = coek::parameter();
        auto a = coek::variable().lower(p).upper(p+1).value(p+2);
        REQUIRE( a.value() == 2 );
        REQUIRE( a.lower() == 0 );
        REQUIRE( a.upper() == 1 );
        p.value(1);
        REQUIRE( a.value() == 3 );
        REQUIRE( a.lower() == 1 );
        REQUIRE( a.upper() == 2 );
    }

    WHEN( "named_with_expr_values" ) {
        auto p = coek::parameter();
        auto a = coek::variable("test").lower(p).upper(p+1).value(p+2);
        REQUIRE( a.value() == 2 );
        REQUIRE( a.lower() == 0 );
        REQUIRE( a.upper() == 1 );
        p.value(1);
        REQUIRE( a.value() == 3 );
        REQUIRE( a.lower() == 1 );
        REQUIRE( a.upper() == 2 );
    }

    WHEN( "copy" ) {
        auto a = coek::variable("test").lower(0).upper(1).value(2);
        coek::Variable b(a);
        REQUIRE( b.value() == 2 );
        REQUIRE( b.lower() == 0 );
        REQUIRE( b.upper() == 1 );
        REQUIRE( b.name() == "test" );
        REQUIRE( a.id() == b.id() );
    }

    WHEN( "equal" ) {
        auto a = coek::variable("test").lower(0).upper(1).value(2);
        auto b = coek::variable();
        b = a;
        REQUIRE( b.value() == 2 );
        REQUIRE( b.lower() == 0 );
        REQUIRE( b.upper() == 1 );
        REQUIRE( b.name() == "test" );
        REQUIRE( a.id() == b.id() );
    }
  }

  SECTION( "index" ) {
    coek::Model model;
    auto a = coek::variable("a").lower(0).upper(1).value(0);
    model.add_variable(a);
    auto b = coek::variable("b").lower(0).upper(1).value(0);
    REQUIRE( a.id() == (b.id() - 1) );
  }

  SECTION( "values" ) {
      WHEN( "variable - 3" ) {
        auto a = coek::variable("a").lower(0).upper(1).value(0);
        REQUIRE( a.value() == 0 );
        a.value(3);
        REQUIRE( a.value() == 3 );
      }

/*
      WHEN( "variable array - 3" ) {
        void* a[2];
        create_variable_array(0, a, 2, false, false, 0.0, 1.0, 0.0, "a");
        variable_value(a[0], 3);

        REQUIRE( variable_value(a[0]) == 3 );
        REQUIRE( variable_value(a[1]) == 0 );
      }

      WHEN( "variable array (2) - 3" ) {
        apival_t model = create_model();

        void* a[2];
        create_variable_array(model, a, 2, false, false, 0.0, 1.0, 0.0, "a");
        variable_value(a[0], 3);

        REQUIRE( variable_value(a[0]) == 3 );
        REQUIRE( variable_value(a[1]) == 0 );
      }
*/
  }

  SECTION( "bounds" ) {
      WHEN( "lb" ) {
        auto a = coek::variable("a").lower(0).upper(1).value(0);
        REQUIRE( a.lower() == 0.0 );
        a.lower(3.0);
        REQUIRE( a.lower() == 3.0 );
      }

      WHEN( "ub" ) {
        auto a = coek::variable("a").lower(0).upper(1).value(0);
        REQUIRE( a.upper() == 1.0 );
        a.upper(3.0);
        REQUIRE( a.upper() == 3.0 );
      }
  }

  SECTION( "properties" ) {
    WHEN( "continuous" ) {
        auto a = coek::variable();
        REQUIRE( a.is_continuous() == true );
        REQUIRE( a.is_binary() == false );
        REQUIRE( a.is_integer() == false );
    }

    WHEN( "binary" ) {
        auto a = coek::variable("a").lower(0).upper(1).value(0).within(coek::Binary);;
        REQUIRE( a.is_continuous() == false );
        REQUIRE( a.is_binary() == true );
        REQUIRE( a.is_integer() == false );
    }

    WHEN( "integer" ) {
        auto a = coek::variable("a").lower(0).upper(10).value(5).within(coek::Integers);;
        REQUIRE( a.is_continuous() == false );
        REQUIRE( a.is_binary() == false );
        REQUIRE( a.is_integer() == true );
    }
  
    WHEN( "fixed" ) {
        auto a = coek::variable("a").lower(0).upper(10).value(5).within(coek::Integers);;
        REQUIRE( a.fixed() == false );
        a.fixed(true);
        REQUIRE( a.fixed() == true );
        REQUIRE( a.value() == 5 );
        a.fix(3);
        REQUIRE( a.fixed() == true );
        REQUIRE( a.value() == 3 );
    }
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    auto q = coek::variable("q");
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_monomial", "[smoke]" ) {

  SECTION( "trivial - int*variable" ) {
    auto v = coek::variable("v").lower(0).upper(0).value(0);
    coek::Expression e = 1*v;
    REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial - int*variable" ) {
    auto v = coek::variable("v").lower(0).upper(0).value(0);
    coek::Expression e = 2*v;
    static std::list<std::string> baseline = {"[", "*", "2", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "trivial - int*parameter" ) {
    auto v = coek::parameter("v").value(1);
    coek::Expression e = 1*v;
    //REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial - int*parameter" ) {
    auto v = coek::parameter("v").value(1);
    coek::Expression e = 2*v;
    static std::list<std::string> baseline = {"[", "*", "2.000", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "trivial - double*variable" ) {
    auto v = coek::variable("v").lower(0).upper(0).value(0);
    coek::Expression e = 1.0*v;
    REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial - double*variable" ) {
    auto v = coek::variable("v").lower(0).upper(0).value(0);
    coek::Expression e = 2.0*v;
    static std::list<std::string> baseline = {"[", "*", "2", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "trivial - double*parameter" ) {
    auto v = coek::parameter("v").value(1);
    coek::Expression e = 1.0*v;
    //REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial - int*parameter" ) {
    auto v = coek::parameter("v").value(1);
    coek::Expression e = 2.0*v;
    static std::list<std::string> baseline = {"[", "*", "2.000", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

/*
  SECTION( "unnamed monomial" ) {
    auto v = coek::variable("v").lower(0).upper(0).value(0);
    coek::Expression e = 2*v;
    static std::list<std::string> baseline = {"[", "*", "2", "x0", "]"};
    REQUIRE( e.to_list() == baseline );
  }
*/

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_expression", "[smoke]" ) {

  SECTION( "constructors" ) {
    WHEN( "simple" ) {
        coek::Expression a;
        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( a.to_list() == baseline );
    }

    WHEN( "double" ) {
        coek::Expression a(1.0);
        static std::list<std::string> baseline = {"1.000"};
        REQUIRE( a.to_list() == baseline );
    }

    WHEN( "equal" ) {
        coek::Expression a(1.0);
        coek::Expression b;
        b = a;
        static std::list<std::string> baseline = {"1.000"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "is_constant" ) {
    WHEN( "constant" ) {
        coek::Expression a(1.0);
        REQUIRE( a.is_constant() == true );
    }

    WHEN( "parameter" ) {
        auto p = coek::parameter().value(1);
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }

    WHEN( "index_parameter" ) {
        auto p = coek::set_index("p");
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }

    WHEN( "variable" ) {
        auto p = coek::variable().lower(0).upper(0).value(0);
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }
  }

  SECTION( "plus-equal" ) {
    WHEN("variable") {
        auto v = coek::variable("v");
        auto p = coek::variable("p").lower(0).upper(0).value(0);
        coek::Expression a = v;
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("double") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        double p=1;
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("int") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        int p=1;
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("parameter") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        auto p = coek::parameter("p");
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("index parameter") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        auto p = coek::set_index("p");
        a += p;
        static std::list<std::string> baseline = {"[", "+", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "minus-equal" ) {
    WHEN("variable") {
        auto v = coek::variable("v");
        auto p = coek::variable("p").lower(0).upper(1).lower(0);
        coek::Expression a = v;
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "p", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("double") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        double p=1;
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "1.000", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("int") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        int p=1;
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "1.000", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("parameter") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        auto p = coek::parameter("p");
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-",  "p", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("index parameter") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        auto p = coek::set_index("p");
        a -= p;
        static std::list<std::string> baseline = {"[", "+", "v", "[", "-", "p", "]", "]"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "times-equal" ) {
    WHEN("variable") {
        auto v = coek::variable("v");
        auto p = coek::variable("p").lower(0).upper(1).value(0);
        coek::Expression a = v;
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("double") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        double p=1;
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("int") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        int p=1;
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("parameter") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        auto p = coek::parameter("p");
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("index parameter") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        auto p = coek::set_index("p");
        a *= p;
        static std::list<std::string> baseline = {"[", "*", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "divide-equal" ) {
    WHEN("variable") {
        auto v = coek::variable("v");
        auto p = coek::variable("p").lower(0).upper(1).value(0);
        coek::Expression a = v;
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("double") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        double p=1;
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("int") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        int p=1;
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "1.000", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("parameter") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        auto p = coek::parameter("p");
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
    WHEN("index parameter") {
        auto v = coek::variable("v");
        coek::Expression a = v;
        auto p = coek::set_index("p");
        a /= p;
        static std::list<std::string> baseline = {"[", "/", "v", "p", "]"};
        REQUIRE( a.to_list() == baseline );
    }
  }

  SECTION( "collect_terms" ) {
    coek::QuadraticExpr repn;
    coek::Model m;
    auto v = coek::variable("v").lower(0).upper(0).value(0);
    coek::Expression a = 1 + v + v*v;

    repn.collect_terms(a);
    std::stringstream sstr;
    sstr << repn;
    REQUIRE( sstr.str() == "Constant: 1\nLinear: \n1 v\nQuadratic: \n1 v v\n");
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_unary_expression", "[smoke]" ) {

  SECTION( "positive" ) {
    WHEN( "param" ) {
        auto q = coek::parameter("q").value(1);
        coek::Expression e;
        e = +q;
        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "iparam" ) {
        auto q = coek::set_index("q");
        coek::Expression e;
        e = +q;
        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "var" ) {
        auto v = coek::variable("v").lower(0).upper(1).value(0);
        coek::Expression e;
        e = +v;
        static std::list<std::string> baseline = {"v"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "expr" ) {
        coek::Expression E(-1);
        coek::Expression e;
        e = +E;
        static std::list<std::string> baseline = {"-1.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "negate" ) {
    WHEN( "param" ) {
        auto q = coek::parameter("q").value(1);
        coek::Expression e;
        e = -q;
        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.value() == -1 );
    }

    WHEN( "iparam" ) {
        auto q = coek::set_index("q");
        coek::Expression e;
        e = -q;
        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "var" ) {
        auto v = coek::variable("v").lower(0).upper(1).value(2);
        coek::Expression e;
        e = -v;
        static std::list<std::string> baseline = {"[", "*", "-1", "v", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.value() == -2 );
    }

    WHEN( "expr" ) {
        coek::Expression E(-1);
        coek::Expression e;
        e = -E;
        static std::list<std::string> baseline = {"1.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_add_expression", "[smoke]" ) {

{
auto a = coek::variable("a").lower(0).upper(1).value(3);
auto b = coek::variable("b").lower(0).upper(1).value(5);
auto c = coek::variable("c").lower(0).upper(1).value(0);
auto d = coek::variable("d").lower(0).upper(1).value(0);
auto z = coek::parameter("z");
auto Z = coek::set_index("Z");
coek::Expression E = b;

  SECTION( "Test simple" ) {
    WHEN("e = a + b") {
        coek::Expression e = a + b;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.value() == 8 );
    }

    WHEN("e = a+b + 2*a") {
        coek::Expression e1 = a + b;
        coek::Expression e2 = 2 * a;
        coek::Expression e = e1 + e2;

        static std::list<std::string> baseline = { "[", "+", "a", "b", "[", "*", "2", "a", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test const + value" ) {
    WHEN( "e = 5 + a" ) {
        coek::Expression e = 5 + a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + a" ) {
        coek::Expression e = 5.0 + a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + z" ) {
        coek::Expression e = 5 + z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + z" ) {
        coek::Expression e = 5.0 + z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + Z" ) {
        coek::Expression e = 5 + Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + Z" ) {
        coek::Expression e = 5.0 + Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + E" ) {
        coek::Expression e = 5 + E;

        static std::list<std::string> baseline = {"[", "+", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + E" ) {
        coek::Expression e = 5.0 + E;

        static std::list<std::string> baseline = {"[", "+", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

  }

  SECTION( "Test var + value" ) {
    WHEN( "e = a + 5" ) {
        coek::Expression e = a + 5;

        static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 5.0" ) {
        coek::Expression e = a + 5.0;

        static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + a" ) {
        coek::Expression e = a + a;

        static std::list<std::string> baseline = {"[", "+", "a", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + z" ) {
        coek::Expression e = a + z;

        static std::list<std::string> baseline = {"[", "+", "a", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + Z" ) {
        coek::Expression e = a + Z;

        static std::list<std::string> baseline = {"[", "+", "a", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + E" ) {
        coek::Expression e = a + E;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test param + value" ) {
    WHEN( "e = z + 5" ) {
        coek::Expression e = z + 5;

        static std::list<std::string> baseline = {"[", "+", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + 5.0" ) {
        coek::Expression e = z + 5.0;

        static std::list<std::string> baseline = {"[", "+", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + a" ) {
        coek::Expression e = z + a;

        static std::list<std::string> baseline = {"[", "+", "z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + z" ) {
        coek::Expression e = z + z;

        static std::list<std::string> baseline = {"[", "+", "z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + Z" ) {
        coek::Expression e = z + Z;

        static std::list<std::string> baseline = {"[", "+", "z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + E" ) {
        coek::Expression e = z + E;

        static std::list<std::string> baseline = {"[", "+", "z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test indexparam + value" ) {
    WHEN( "e = Z + 5" ) {
        coek::Expression e = Z + 5;

        static std::list<std::string> baseline = {"[", "+", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + 5.0" ) {
        coek::Expression e = Z + 5.0;

        static std::list<std::string> baseline = {"[", "+", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + a" ) {
        coek::Expression e = Z + a;

        static std::list<std::string> baseline = {"[", "+", "Z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + z" ) {
        coek::Expression e = Z + z;

        static std::list<std::string> baseline = {"[", "+", "Z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + Z" ) {
        coek::Expression e = Z + Z;

        static std::list<std::string> baseline = {"[", "+", "Z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + E" ) {
        coek::Expression e = Z + E;

        static std::list<std::string> baseline = {"[", "+", "Z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test expr + value" ) {
    WHEN( "e = E + 5" ) {
        coek::Expression e = E + 5;

        static std::list<std::string> baseline = {"[", "+", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + 5.0" ) {
        coek::Expression e = E + 5.0;

        static std::list<std::string> baseline = {"[", "+", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + a" ) {
        coek::Expression e = E + a;

        static std::list<std::string> baseline = {"[", "+", "b", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + z" ) {
        coek::Expression e = E + z;

        static std::list<std::string> baseline = {"[", "+", "b", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + Z" ) {
        coek::Expression e = E + Z;

        static std::list<std::string> baseline = {"[", "+", "b", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + E" ) {
        coek::Expression e = E + E;

        static std::list<std::string> baseline = {"[", "+", "b", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedSum" ) {

    WHEN( "e = (a+b)+5" ) {
        coek::Expression e = (a+b)+5;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5+(a+b)" ) {
        coek::Expression e = 5+(a+b);

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "+", "a", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a+b)+5.0" ) {
        coek::Expression e = (a+b)+5.0;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0+(a+b)" ) {
        coek::Expression e = 5.0+(a+b);

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "+", "a", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a+b)+c" ) {
        coek::Expression e = (a+b)+c;

        static std::list<std::string> baseline = {"[", "+", "a", "b", "c", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = c+(a+b)" ) {
        coek::Expression e = c+(a+b);

        static std::list<std::string> baseline = {"[", "+", "c", "[", "+", "a", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a+b)+(c+d)" ) {
        coek::Expression e = (a+b)+(c+d);

        static std::list<std::string> baseline = {"[", "+", "a", "b", "[", "+", "c", "d", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedSum" ) {

    WHEN( "e = 2*(a+b)+c" ) {
        coek::Expression e = 2*(a+b)+c;

        static std::list<std::string> baseline = {"[", "+", "[", "*", "2.000", "[", "+", "a", "b", "]", "]", "c", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 3*(2*(a+b)+c)" ) {
        coek::Expression e = 3*(2*(a+b)+c);

        static std::list<std::string> baseline = {"[", "*", "3.000", "[", "+", "[", "*", "2.000", "[", "+", "a", "b", "]", "]", "c", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialSum" ) {
    auto q = coek::parameter("q").value(1);
    auto Q = coek::parameter("Q");
    auto Z = coek::set_index("Z");
    coek::Expression E = a;

    WHEN( "e = 0 + a" ) {
        coek::Expression e = 0 + a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + a" ) {
        coek::Expression e = 0.0 + a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 + q" ) {
        coek::Expression e = 0 + q;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + q" ) {
        coek::Expression e = 0.0 + q;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 + Z" ) {
        coek::Expression e = 0 + Z;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + Z" ) {
        coek::Expression e = 0.0 + Z;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 + E" ) {
        coek::Expression e = 0 + E;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + E" ) {
        coek::Expression e = 0.0 + E;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 0" ) {
        coek::Expression e = a + 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 0.0" ) {
        coek::Expression e = a + 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q + 0" ) {
        coek::Expression e = q + 0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q + 0.0" ) {
        coek::Expression e = q + 0.0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + 0" ) {
        coek::Expression e = Z + 0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + 0.0" ) {
        coek::Expression e = Z + 0.0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + 0" ) {
        coek::Expression e = E + 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E + 0.0" ) {
        coek::Expression e = E + 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test sum with zero-valued parameter" ) {
    auto q = coek::parameter("q").value(1);
    auto Q = coek::parameter("Q");

    WHEN( "e = a + q{0}" ) {
        coek::Expression e = a + q;

        static std::list<std::string> baseline = {"[","+","a","q","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q{0} + a" ) {
        coek::Expression e = q + a;

        static std::list<std::string> baseline = {"[","+","q","a","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + Q{0.0}" ) {
        coek::Expression e = a + Q;

        static std::list<std::string> baseline = {"[","+","a","Q","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} + a" ) {
        coek::Expression e = Q + a;

        static std::list<std::string> baseline = {"[","+","Q","a","]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedTrivialProduct" ) {

    WHEN( "e = a*5 + b" ) {
        coek::Expression e = a*5 + b;

        static std::list<std::string> baseline = {"[", "+", "[", "*", "5", "a", "]", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = b + a*5" ) {
        coek::Expression e = b + a*5;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "5", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a*5 + (b+c)" ) {
        coek::Expression e = a*5 + (b+c);

        static std::list<std::string> baseline = { "[", "+", "[", "*", "5", "a", "]", "[", "+", "b", "c", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (b+c) + a*5" ) {
        coek::Expression e = (b+c) + a*5;

        static std::list<std::string> baseline = { "[", "+", "b", "c", "[", "*", "5", "a", "]", "]" };
        REQUIRE( e.to_list() == baseline );
    }
  }

}

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_minus_expression", "[smoke]" ) {

{
auto a = coek::variable("a").lower(0).upper(1).value(3);
auto b = coek::variable("b").lower(0).upper(1).value(5);
auto c = coek::variable("c").lower(0).upper(1).value(0);
auto d = coek::variable("d").lower(0).upper(1).value(0);

auto z = coek::parameter("z").value(1.0);
auto Z = coek::set_index("Z");
coek::Expression f;
coek::Expression E = b;

  SECTION( "Test simpleDiff" ) {
    coek::Expression e = a - b;

    static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "]" };
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.value() == -2 );
  }

  SECTION( "Test termDiff" ) {
    coek::Expression e = 5 - 2*a;

    static std::list<std::string> baseline = { "[", "+", "5.000", "[", "*", "-2", "a", "]", "]" };
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test const - value" ) {
    WHEN( "e = 5 - a" ) {
        coek::Expression e = 5 - a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 - a" ) {
        coek::Expression e = 5.0 - a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 - z" ) {
        coek::Expression e = 5 - z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 - z" ) {
        coek::Expression e = 5.0 - z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 - Z" ) {
        coek::Expression e = 5 - Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 - Z" ) {
        coek::Expression e = 5.0 - Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 - E" ) {
        coek::Expression e = 5 - E;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 - E" ) {
        coek::Expression e = 5.0 - E;

        static std::list<std::string> baseline = {"[", "+", "5.000", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

  }

  SECTION( "Test var - value" ) {
    WHEN( "e = a - 5" ) {
        coek::Expression e = a - 5;

        static std::list<std::string> baseline = {"[", "+", "a", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 5.0" ) {
        coek::Expression e = a - 5.0;

        static std::list<std::string> baseline = {"[", "+", "a", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - a" ) {
        coek::Expression e = a - a;

        static std::list<std::string> baseline = {"[", "+", "a", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - z" ) {
        coek::Expression e = a - z;

        static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - Z" ) {
        coek::Expression e = a - Z;

        static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - E" ) {
        coek::Expression e = a - E;

        static std::list<std::string> baseline = {"[", "+", "a", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test param - value" ) {
    WHEN( "e = z - 5" ) {
        coek::Expression e = z - 5;

        static std::list<std::string> baseline = {"[", "+", "z", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - 5.0" ) {
        coek::Expression e = z - 5.0;

        static std::list<std::string> baseline = {"[", "+", "z", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - a" ) {
        coek::Expression e = z - a;

        static std::list<std::string> baseline = {"[", "+", "z", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - z" ) {
        coek::Expression e = z - z;

        static std::list<std::string> baseline = {"[", "+", "z", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - Z" ) {
        coek::Expression e = z - Z;

        static std::list<std::string> baseline = {"[", "+", "z", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z - E" ) {
        coek::Expression e = z - E;

        static std::list<std::string> baseline = {"[", "+", "z", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test indexparam - value" ) {
    WHEN( "e = Z - 5" ) {
        coek::Expression e = Z - 5;

        static std::list<std::string> baseline = {"[", "+", "Z", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - 5.0" ) {
        coek::Expression e = Z - 5.0;

        static std::list<std::string> baseline = {"[", "+", "Z", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - a" ) {
        coek::Expression e = Z - a;

        static std::list<std::string> baseline = {"[", "+", "Z", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - z" ) {
        coek::Expression e = Z - z;

        static std::list<std::string> baseline = {"[", "+", "Z", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - Z" ) {
        coek::Expression e = Z - Z;

        static std::list<std::string> baseline = {"[", "+", "Z", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - E" ) {
        coek::Expression e = Z - E;

        static std::list<std::string> baseline = {"[", "+", "Z", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test expr - value" ) {
    WHEN( "e = E - 5" ) {
        coek::Expression e = E - 5;

        static std::list<std::string> baseline = {"[", "+", "b", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - 5.0" ) {
        coek::Expression e = E - 5.0;

        static std::list<std::string> baseline = {"[", "+", "b", "-5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - a" ) {
        coek::Expression e = E - a;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "-1", "a", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - z" ) {
        coek::Expression e = E - z;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "-", "z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - Z" ) {
        coek::Expression e = E - Z;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "-", "Z", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - E" ) {
        coek::Expression e = E - E;

        static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "-1", "b", "]", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedDiff" ) {

    WHEN( "e = (a-b)-5" ) {
      coek::Expression e = (a-b) - 5;

      static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "-5.000", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5-(a-b)" ) {
      coek::Expression e = 5 - (a-b);

      static std::list<std::string> baseline = { "[", "+", "5.000", "[", "-", "[", "+", "a", "[", "*", "-1", "b", "]", "]", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a-b)-c" ) {
      coek::Expression e = (a-b) - c;

      static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "[", "*", "-1", "c", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a-b)-(c-d)" ) {
      coek::Expression e = (a-b) - (c - d);

      static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "[", "-", "[", "+", "c", "[", "*", "-1", "d", "]", "]", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test sumOf_nestedTrivialProduct2" ) {

    WHEN( "a*5 - b" ) {
      coek::Expression e = a*5 - b;
      
      static std::list<std::string> baseline = { "[", "+", "[", "*", "5", "a", "]", "[", "*", "-1", "b", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "b - a*5" ) {
      coek::Expression e = b - a*5;
      
      static std::list<std::string> baseline = { "[", "+", "b", "[", "*", "-5", "a", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "a*5 - (b-c)" ) {
      coek::Expression e = a*5 - (b-c);
      
      static std::list<std::string> baseline = {"[", "+", "[", "*", "5", "a", "]", "[", "-", "[", "+", "b", "[", "*", "-1", "c", "]", "]", "]", "]" };
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "(b-c) - a*5" ) {
      coek::Expression e = (b-c) - a*5;
      
      static std::list<std::string> baseline = {"[", "+", "b", "[", "*", "-1", "c", "]", "[", "*", "-5", "a", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialDiff" ) {
    auto q = coek::parameter("q").value(1);
    auto Q = coek::parameter("Q");
    auto Z = coek::set_index("Z");
    coek::Expression E = a;

    WHEN( "e = 0 - a" ) {
        coek::Expression e = 0 - a;

        static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - a" ) {
        coek::Expression e = 0.0 - a;

        static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 - q" ) {
        coek::Expression e = 0 - q;

        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - q" ) {
        coek::Expression e = 0.0 - q;

        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 - Z" ) {
        coek::Expression e = 0 - Z;

        static std::list<std::string> baseline = {"[", "-", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - Z" ) {
        coek::Expression e = 0.0 - Z;

        static std::list<std::string> baseline = {"[", "-", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 - E" ) {
        coek::Expression e = 0 - E;

        static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - E" ) {
        coek::Expression e = 0.0 - E;

        static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 0" ) {
        coek::Expression e = a - 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 0.0" ) {
        coek::Expression e = a - 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q - 0" ) {
        coek::Expression e = q - 0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q - 0.0" ) {
        coek::Expression e = q - 0.0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - 0" ) {
        coek::Expression e = Z - 0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z - 0.0" ) {
        coek::Expression e = Z - 0.0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - 0" ) {
        coek::Expression e = E - 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E - 0.0" ) {
        coek::Expression e = E - 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test diff with zero-valued parameter" ) {
    auto q = coek::parameter("q");
    auto Q = coek::parameter("Q");

    WHEN( "e = a - q{0}" ) {
      coek::Expression e = a - q;

      static std::list<std::string> baseline = {"[","+","a","[","-","q","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q{0} - a" ) {
      coek::Expression e = q - a;

      static std::list<std::string> baseline = {"[","+","q","[","*","-1","a","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q_{0} - a" ) {
      coek::Expression e = 0*q - a;

      static std::list<std::string> baseline = {"[", "+", "0.000", "[", "*", "-1", "a", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - Q{0.0}" ) {
      coek::Expression e = a - Q;

      static std::list<std::string> baseline = {"[","+","a","[","-","Q","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} - a" ) {
      coek::Expression e = Q - a;

      static std::list<std::string> baseline = {"[","+","Q","[","*","-1","a","]","]"};
      REQUIRE( e.to_list() == baseline );
    }
  }


}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_neg_expression", "[smoke]" ) {

  SECTION( "Test negation_param" ) {
    auto p = coek::parameter("p").value(2);

    WHEN( "e = -p" ) {
      coek::Expression e = -p;

      static std::list<std::string> baseline = {"[", "-", "p", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.value() == -2 );
    }

    WHEN( "e = -(-p)" ) {
      coek::Expression e = -(-p);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "p", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test negation_terms" ) {
    auto p = coek::parameter("p").value(2);
    auto v = coek::variable("v").lower(0).upper(1).value(0);

    WHEN( "e = - p*v" ) {
      coek::Expression e = - p*v;

      static std::list<std::string> baseline = {"[", "*", "[", "-", "p", "]", "v", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = -(- p*v)" ) {
      coek::Expression e = -(- p*v);

      static std::list<std::string> baseline = {"[", "-", "[", "*", "[", "-", "p", "]", "v", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = - 5*v" ) {
      coek::Expression e = - 5*v;

      static std::list<std::string> baseline = {"[", "*", "-5", "v", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

#ifdef DEBUG
  REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_mul_expression", "[smoke]" ) {

{
auto a = coek::variable("a").lower(0).upper(1).value(3);
auto b = coek::variable("b").lower(0).upper(1).value(5);
auto c = coek::variable("c").lower(0).upper(1).value(0);
auto d = coek::variable("d").lower(0).upper(1).value(0);

auto z = coek::parameter("z");
auto Z = coek::set_index("Z");
coek::Expression f;
coek::Expression E = b;

  SECTION( "Test simpleProduct" ) {
    coek::Expression e = a*b;

    static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.value() == 15 );
  }

  SECTION( "Test const * value" ) {
    WHEN( "e = 5 * a" ) {
        coek::Expression e = 5 * a;

        static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 * a" ) {
        coek::Expression e = 5.0 * a;

        static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 * z" ) {
        coek::Expression e = 5 * z;

        static std::list<std::string> baseline = {"[", "*", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 * z" ) {
        coek::Expression e = 5.0 * z;

        static std::list<std::string> baseline = {"[", "*", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 * Z" ) {
        coek::Expression e = 5 * Z;

        static std::list<std::string> baseline = {"[", "*", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 * Z" ) {
        coek::Expression e = 5.0 * Z;

        static std::list<std::string> baseline = {"[", "*", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 * E" ) {
        coek::Expression e = 5 * E;

        static std::list<std::string> baseline = {"[", "*", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 * E" ) {
        coek::Expression e = 5.0 * E;

        static std::list<std::string> baseline = {"[", "*", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

  }

  SECTION( "Test var * value" ) {
    WHEN( "e = a * 5" ) {
        coek::Expression e = a * 5;

        static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 5.0" ) {
        coek::Expression e = a * 5.0;

        static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * a" ) {
        coek::Expression e = a * a;

        static std::list<std::string> baseline = {"[", "*", "a", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * z" ) {
        coek::Expression e = a * z;

        static std::list<std::string> baseline = {"[", "*", "a", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * Z" ) {
        coek::Expression e = a * Z;

        static std::list<std::string> baseline = {"[", "*", "a", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * E" ) {
        coek::Expression e = a * E;

        static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test param * value" ) {
    WHEN( "e = z * 5" ) {
        coek::Expression e = z * 5;

        static std::list<std::string> baseline = {"[", "*", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * 5.0" ) {
        coek::Expression e = z * 5.0;

        static std::list<std::string> baseline = {"[", "*", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * a" ) {
        coek::Expression e = z * a;

        static std::list<std::string> baseline = {"[", "*", "z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * z" ) {
        coek::Expression e = z * z;

        static std::list<std::string> baseline = {"[", "*", "z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * Z" ) {
        coek::Expression e = z * Z;

        static std::list<std::string> baseline = {"[", "*", "z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z * E" ) {
        coek::Expression e = z * E;

        static std::list<std::string> baseline = {"[", "*", "z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test indexparam * value" ) {
    WHEN( "e = Z * 5" ) {
        coek::Expression e = Z * 5;

        static std::list<std::string> baseline = {"[", "*", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 5.0" ) {
        coek::Expression e = Z * 5.0;

        static std::list<std::string> baseline = {"[", "*", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * a" ) {
        coek::Expression e = Z * a;

        static std::list<std::string> baseline = {"[", "*", "Z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * z" ) {
        coek::Expression e = Z * z;

        static std::list<std::string> baseline = {"[", "*", "Z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * Z" ) {
        coek::Expression e = Z * Z;

        static std::list<std::string> baseline = {"[", "*", "Z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * E" ) {
        coek::Expression e = Z * E;

        static std::list<std::string> baseline = {"[", "*", "Z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test expr * value" ) {
    WHEN( "e = E * 5" ) {
        coek::Expression e = E * 5;

        static std::list<std::string> baseline = {"[", "*", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 5.0" ) {
        coek::Expression e = E * 5.0;

        static std::list<std::string> baseline = {"[", "*", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * a" ) {
        coek::Expression e = E * a;

        static std::list<std::string> baseline = {"[", "*", "b", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * z" ) {
        coek::Expression e = E * z;

        static std::list<std::string> baseline = {"[", "*", "b", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * Z" ) {
        coek::Expression e = E * Z;

        static std::list<std::string> baseline = {"[", "*", "b", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * E" ) {
        coek::Expression e = E * E;

        static std::list<std::string> baseline = {"[", "*", "b", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedProduct" ) {
    WHEN( "e = (a*b)*5" ) {
      coek::Expression e = (a*b)*5;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      coek::Expression e = 5*(a*b);

      static std::list<std::string> baseline = {"[", "*", "5.000", "[", "*", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a*b)*c" ) {
      coek::Expression e = (a*b)*c;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "c", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      coek::Expression e = c*(a*b);

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      coek::Expression e = (a*b)*(c*d);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedProduct2" ) {

    WHEN( "e = (c+(a+b)) * ((a+b)+d)" ) {
      coek::Expression e = (c+(a+b))*((a+b)+d);

      static std::list<std::string> baseline = {"[", "*", "[", "+", "c", "[", "+", "a", "b", "]", "]", "[", "+", "a", "b", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (c*(a+b)) * ((a+b)*d)" ) {
      coek::Expression e = (c*(a+b))*((a+b)*d);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "c", "[", "+", "a", "b", "]", "]", "[", "*", "[", "+", "a", "b", "]", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nestedProduct3" ) {

    WHEN( "e = (3*b)*5" ) {
      coek::Expression e = (3*b)*5;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a*b)*5" ) {
      coek::Expression e = (a*b)*5;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*(3*b)" ) {
      coek::Expression e = 5*(3*b);

      static std::list<std::string> baseline = {"[", "*", "5.000", "[", "*", "3", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*(a*b)" ) {
      coek::Expression e = 5*(a*b);

      static std::list<std::string> baseline = {"[", "*", "5.000", "[", "*", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (3*b)*c" ) {
      coek::Expression e = (3*b)*c;

      static std::list<std::string> baseline = {"[", "*", "[", "*", "3", "b", "]", "c", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = c*(a*b)" ) {
      coek::Expression e = c*(a*b);

      static std::list<std::string> baseline = {"[", "*", "c", "[", "*", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a*b)*(c*d)" ) {
      coek::Expression e = (a*b)*(c*d);

      static std::list<std::string> baseline = {"[", "*", "[", "*", "a", "b", "]", "[", "*", "c", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialMul - 0" ) {
    auto q = coek::parameter("q").value(1);
    auto Q = coek::parameter("Q");
    auto Z = coek::set_index("Z");
    coek::Expression E = a;

    WHEN( "e = 0 * a" ) {
        coek::Expression e = 0 * a;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * a" ) {
        coek::Expression e = 0.0 * a;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 * q" ) {
        coek::Expression e = 0 * q;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * q" ) {
        coek::Expression e = 0.0 * q;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 * Z" ) {
        coek::Expression e = 0 * Z;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * Z" ) {
        coek::Expression e = 0.0 * Z;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 * E" ) {
        coek::Expression e = 0 * E;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * E" ) {
        coek::Expression e = 0.0 * E;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 0" ) {
        coek::Expression e = a * 0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 0.0" ) {
        coek::Expression e = a * 0.0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q * 0" ) {
        coek::Expression e = q * 0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q * 0.0" ) {
        coek::Expression e = q * 0.0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 0" ) {
        coek::Expression e = Z * 0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 0.0" ) {
        coek::Expression e = Z * 0.0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 0" ) {
        coek::Expression e = E * 0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 0.0" ) {
        coek::Expression e = E * 0.0;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialMul - 1" ) {
    auto q = coek::parameter("q").value(1);
    auto Q = coek::parameter("Q");
    auto Z = coek::set_index("Z");
    coek::Expression E = a;

    WHEN( "e = 1 * a" ) {
        coek::Expression e = 1 * a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * a" ) {
        coek::Expression e = 1.0 * a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 * q" ) {
        coek::Expression e = 1 * q;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * q" ) {
        coek::Expression e = 1.0 * q;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 * Z" ) {
        coek::Expression e = 1 * Z;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * Z" ) {
        coek::Expression e = 1.0 * Z;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 * E" ) {
        coek::Expression e = 1 * E;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * E" ) {
        coek::Expression e = 1.0 * E;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 1" ) {
        coek::Expression e = a * 1;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 1.0" ) {
        coek::Expression e = a * 1.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q * 1" ) {
        coek::Expression e = q * 1;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q * 1.0" ) {
        coek::Expression e = q * 1.0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 1" ) {
        coek::Expression e = Z * 1;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z * 1.0" ) {
        coek::Expression e = Z * 1.0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 1" ) {
        coek::Expression e = E * 1;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E * 1.0" ) {
        coek::Expression e = E * 1.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test mul with trivial parameter" ) {
    auto q = coek::parameter("q");
    auto Q = coek::parameter("Q");
    auto r = coek::parameter("r").value(1);
    auto R = coek::parameter("R").value(1.0);

    WHEN( "e = a * q{0}" ) {
      coek::Expression e = a * q;

      static std::list<std::string> baseline = {"[","*","a","q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q{0} * a" ) {
      coek::Expression e = q * a;

      static std::list<std::string> baseline = {"[","*","q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * Q{0.0}" ) {
      coek::Expression e = a * Q;

      static std::list<std::string> baseline = {"[","*","a","Q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} * a" ) {
      coek::Expression e = Q * a;

      static std::list<std::string> baseline = {"[","*","Q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * r{1}" ) {
      coek::Expression e = a * r;

      static std::list<std::string> baseline = {"[","*","a","r","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = r{1} * a" ) {
      coek::Expression e = r * a;

      static std::list<std::string> baseline = {"[","*","r","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * R{1.0}" ) {
      coek::Expression e = a * R;

      static std::list<std::string> baseline = {"[","*","a","R","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = R{1.0} * a" ) {
      coek::Expression e = R * a;

      static std::list<std::string> baseline = {"[","*","R","a","]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_div_expression", "[smoke]" ) {

{
auto a = coek::variable("a").lower(0).upper(1).value(3);
auto b = coek::variable("b").lower(0).upper(1).value(5);
auto c = coek::variable("c").lower(0).upper(1).value(0);
auto d = coek::variable("d").lower(0).upper(1).value(0);

auto z = coek::parameter("z");
auto Z = coek::set_index("Z");
coek::Expression f;
coek::Expression E = b;

  SECTION( "Test simpleDivision" ) {
    coek::Expression e = a/b;

    static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.value() == 0.6 );
  }

  SECTION( "Test const / value" ) {
    WHEN( "e = 5 / a" ) {
        coek::Expression e = 5 / a;

        static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 / a" ) {
        coek::Expression e = 5.0 / a;

        static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 / z" ) {
        coek::Expression e = 5 / z;

        static std::list<std::string> baseline = {"[", "/", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 / z" ) {
        coek::Expression e = 5.0 / z;

        static std::list<std::string> baseline = {"[", "/", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 / Z" ) {
        coek::Expression e = 5 / Z;

        static std::list<std::string> baseline = {"[", "/", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 / Z" ) {
        coek::Expression e = 5.0 / Z;

        static std::list<std::string> baseline = {"[", "/", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 / E" ) {
        coek::Expression e = 5 / E;

        static std::list<std::string> baseline = {"[", "/", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 / E" ) {
        coek::Expression e = 5.0 / E;

        static std::list<std::string> baseline = {"[", "/", "5.000", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }

  }

  SECTION( "Test var / value" ) {
    WHEN( "e = a / 5" ) {
        coek::Expression e = a / 5;

        static std::list<std::string> baseline = {"[", "*", "0.2", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / 5.0" ) {
        coek::Expression e = a / 5.0;

        static std::list<std::string> baseline = {"[", "*", "0.2", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / a" ) {
        coek::Expression e = a / a;

        static std::list<std::string> baseline = {"[", "/", "a", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / z" ) {
        coek::Expression e = a / z;

        static std::list<std::string> baseline = {"[", "/", "a", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / Z" ) {
        coek::Expression e = a / Z;

        static std::list<std::string> baseline = {"[", "/", "a", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / E" ) {
        coek::Expression e = a / E;

        static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test param / value" ) {
    WHEN( "e = z / 5" ) {
        coek::Expression e = z / 5;

        static std::list<std::string> baseline = {"[", "/", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / 5.0" ) {
        coek::Expression e = z / 5.0;

        static std::list<std::string> baseline = {"[", "/", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / a" ) {
        coek::Expression e = z / a;

        static std::list<std::string> baseline = {"[", "/", "z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / z" ) {
        coek::Expression e = z / z;

        static std::list<std::string> baseline = {"[", "/", "z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / Z" ) {
        coek::Expression e = z / Z;

        static std::list<std::string> baseline = {"[", "/", "z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z / E" ) {
        coek::Expression e = z / E;

        static std::list<std::string> baseline = {"[", "/", "z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test indexparam / value" ) {
    WHEN( "e = Z / 5" ) {
        coek::Expression e = Z / 5;

        static std::list<std::string> baseline = {"[", "/", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / 5.0" ) {
        coek::Expression e = Z / 5.0;

        static std::list<std::string> baseline = {"[", "/", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / a" ) {
        coek::Expression e = Z / a;

        static std::list<std::string> baseline = {"[", "/", "Z", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / z" ) {
        coek::Expression e = Z / z;

        static std::list<std::string> baseline = {"[", "/", "Z", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / Z" ) {
        coek::Expression e = Z / Z;

        static std::list<std::string> baseline = {"[", "/", "Z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z / E" ) {
        coek::Expression e = Z / E;

        static std::list<std::string> baseline = {"[", "/", "Z", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test expr / value" ) {
    WHEN( "e = E / 5" ) {
        coek::Expression e = E / 5;

        static std::list<std::string> baseline = {"[", "/", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / 5.0" ) {
        coek::Expression e = E / 5.0;

        static std::list<std::string> baseline = {"[", "/", "b", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / a" ) {
        coek::Expression e = E / a;

        static std::list<std::string> baseline = {"[", "/", "b", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / z" ) {
        coek::Expression e = E / z;

        static std::list<std::string> baseline = {"[", "/", "b", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / Z" ) {
        coek::Expression e = E / Z;

        static std::list<std::string> baseline = {"[", "/", "b", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E / E" ) {
        coek::Expression e = E / E;

        static std::list<std::string> baseline = {"[", "/", "b", "b", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test nested Division" ) {

    WHEN( "e = (3*b)/5" ) {
      coek::Expression e = (3*b)/5;

      static std::list<std::string> baseline = {"[", "/", "[", "*", "3", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a/b)/5" ) {
      coek::Expression e = (a/b)/5;

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5/(a/b)" ) {
      coek::Expression e = 5/(a/b);

      static std::list<std::string> baseline = {"[", "/", "5.000", "[", "/", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a/b)/c" ) {
      coek::Expression e = (a/b)/c;

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "c", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = c/(a/b)" ) {
      coek::Expression e = c/(a/b);

      static std::list<std::string> baseline = {"[", "/", "c", "[", "/", "a", "b", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a/b)/(c/d)" ) {
      coek::Expression e = (a/b)/(c/d);

      static std::list<std::string> baseline = {"[", "/", "[", "/", "a", "b", "]", "[", "/", "c", "d", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialDiv - 0" ) {
    auto q = coek::parameter("q").value(1);
    auto Z = coek::set_index("Z");
    coek::Expression E = a;

    WHEN( "e = 0 / a" ) {
        coek::Expression e = 0 / a;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / a" ) {
        coek::Expression e = 0.0 / a;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 / q" ) {
        coek::Expression e = 0 / q;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / q" ) {
        coek::Expression e = 0.0 / q;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 / Z" ) {
        coek::Expression e = 0 / Z;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / Z" ) {
        coek::Expression e = 0.0 / Z;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 / E" ) {
        coek::Expression e = 0 / E;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / E" ) {
        coek::Expression e = 0.0 / E;

        static std::list<std::string> baseline = {"0.000"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / 0" ) {
      REQUIRE_THROWS_WITH( a/0,
        "Division by zero.");
    }

    WHEN( "e = a / 0.0" ) {
      REQUIRE_THROWS_WITH( a/0.0,
        "Division by zero.");
    }

    WHEN( "e = q / 0" ) {
      REQUIRE_THROWS_WITH( q/0,
        "Division by zero.");
    }

    WHEN( "e = q / 0.0" ) {
      REQUIRE_THROWS_WITH( q/0.0,
        "Division by zero.");
    }

    WHEN( "e = Z / 0" ) {
      REQUIRE_THROWS_WITH( Z/0,
        "Division by zero.");
    }

    WHEN( "e = Z / 0.0" ) {
      REQUIRE_THROWS_WITH( Z/0.0,
        "Division by zero.");
    }

    WHEN( "e = E / 0" ) {
      REQUIRE_THROWS_WITH( E/0,
        "Division by zero.");
    }

    WHEN( "e = E / 0.0" ) {
      REQUIRE_THROWS_WITH( E/0.0,
        "Division by zero.");
    }
  }

  SECTION( "Test trivialDiv - 1" ) {
    auto q = coek::parameter("q").value(1);
    auto Z = coek::set_index("Z");

    WHEN( "e = a/1" ) {
        coek::Expression e = a/1;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/1.0" ) {
        coek::Expression e = a/1.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q/1" ) {
        coek::Expression e = q/1;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q/1.0" ) {
        coek::Expression e = q/1.0;

        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/1" ) {
        coek::Expression e = Z/1;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/1.0" ) {
        coek::Expression e = Z/1.0;

        static std::list<std::string> baseline = {"Z"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E/1" ) {
        coek::Expression e = E/1;

        static std::list<std::string> baseline = {"b"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = E/1.0" ) {
        coek::Expression e = E/1.0;

        static std::list<std::string> baseline = {"b"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test div with trivial parameter" ) {

    auto q = coek::parameter("q");
    auto Q = coek::parameter("Q");
    auto r = coek::parameter("r").value(1);
    auto R = coek::parameter("R").value(1.0);

    WHEN( "e = q{0} / a" ) {
      coek::Expression e = q/a;

      static std::list<std::string> baseline = {"[","/","q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} / a" ) {
      coek::Expression e = Q/a;

      static std::list<std::string> baseline = {"[","/","Q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/ r{1}" ) {
      coek::Expression e = a/r;

      static std::list<std::string> baseline = {"[","/","a","r","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / R{1.0}" ) {
      coek::Expression e = a/R;

      static std::list<std::string> baseline = {"[","/","a","R","]"};
      REQUIRE( e.to_list() == baseline );
    }

  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "intrinsics", "[smoke]" ) {

{
auto v = coek::variable("v").lower(0).upper(1).value(0);
auto p = coek::parameter("p");

  SECTION( "Test abs" ) {
    {
    coek::Expression constant(-1);
    REQUIRE( constant.is_constant() );
    coek::Expression e = abs(constant);
    REQUIRE( e.value() == 1.0 );
    }

    coek::Expression e = abs(v);
    v.value(1);
    REQUIRE( e.value() == 1.0 );
    v.value(-1);
    REQUIRE( e.value() == 1.0 );

    static std::list<std::string> baseline = {"[", "abs", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test abs - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = abs(constant);
    REQUIRE( e.value() == 0.0 );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test ceil" ) {
    coek::Expression e = ceil(v);
    v.value(1.5);
    REQUIRE( e.value() == 2.0 );
    v.value(-1.5);
    REQUIRE( e.value() == -1.0 );

    static std::list<std::string> baseline = {"[", "ceil", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test ceil - constant" ) {
    coek::Expression constant(1.5);
    coek::Expression e = ceil(constant);
    REQUIRE( e.value() == 2.0 );

    static std::list<std::string> baseline = {"2.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test floor" ) {
    coek::Expression e = floor(v);
    v.value(1.5);
    REQUIRE( e.value() == 1.0 );
    v.value(-1.5);
    REQUIRE( e.value() == -2.0 );

    static std::list<std::string> baseline = {"[", "floor", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test floor - constant" ) {
    coek::Expression constant(1.5);
    coek::Expression e = floor(constant);
    REQUIRE( e.value() == 1.0 );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test exp" ) {
    coek::Expression e = exp(v);
    v.value(1);
    REQUIRE( e.value() == Approx(E) );
    v.value(0);
    REQUIRE( e.value() == 1.0 );

    static std::list<std::string> baseline = {"[", "exp", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test exp - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = exp(constant);
    REQUIRE( e.value() == Approx(E) );

    static std::list<std::string> baseline = {"2.718"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log" ) {
    coek::Expression e = log(v);
    v.value(1);
    REQUIRE( e.value() == Approx(0.0) );
    v.value( exp(1.0) );
    REQUIRE( e.value() == 1.0 );

    static std::list<std::string> baseline = {"[", "log", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = log(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log10" ) {
    coek::Expression e = log10(v);
    v.value(1);
    REQUIRE( e.value() == Approx(0.0) );
    v.value(10);
    REQUIRE( e.value() == 1.0 );

    static std::list<std::string> baseline = {"[", "log10", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log10 - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = log(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test pow" ) {
    {
    coek::Expression constant0(0);
    coek::Expression e = pow(constant0,p);
    p.value(1);
    REQUIRE( e.value() == Approx(0.0) );
    }
    {
    coek::Expression constant1(1);
    coek::Expression e = pow(constant1,p);
    p.value(1);
    REQUIRE( e.value() == Approx(1.0) );
    }
    {
    coek::Expression constant2(2);
    coek::Expression constant1(1);
    coek::Expression e = pow(constant2,constant1);
    REQUIRE( e.value() == Approx(2.0) );
    }
    {
    v.value(2);
    coek::Expression constant1(1);
    coek::Expression e = pow(v,constant1);
    REQUIRE( e.value() == Approx(2.0) );
    }

    coek::Expression e = pow(v,p);
    v.value(2);
    p.value(0);
    REQUIRE( e.value() == Approx(1.0) );
    p.value(1);
    REQUIRE( e.value() == Approx(2.0) );

    static std::list<std::string> baseline = {"[", "pow", "v", "p", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test pow - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = pow(v,constant);
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sqrt" ) {
    coek::Expression e = sqrt(v);
    v.value(1);
    REQUIRE( e.value() == Approx(1.0) );
    v.value(4);
    REQUIRE( e.value() == 2.0 );

    static std::list<std::string> baseline = {"[", "sqrt", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sqrt - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = sqrt(constant);
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sin" ) {
    coek::Expression e = sin(v);
    v.value(0);
    REQUIRE( e.value() == Approx(0.0) );
    v.value(PI/2.0);
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "sin", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sin - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = sqrt(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cos" ) {
    coek::Expression e = cos(v);
    v.value(0);
    REQUIRE( e.value() == Approx(1.0) );
    v.value(PI/2.0);
    REQUIRE( e.value() == Approx(0.0).margin(1e-7) );

    static std::list<std::string> baseline = {"[", "cos", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cos - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = cos(constant);
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tan" ) {
    coek::Expression e = tan(v);
    v.value(0);
    REQUIRE( e.value() == Approx(0.0) );
    v.value(PI/4.0);
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "tan", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tan - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = tan(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asin" ) {
    coek::Expression e = asin(v);
    v.value(0);
    REQUIRE( e.value() == Approx(0.0) );
    v.value(1);
    REQUIRE( e.value() == Approx(PI/2.0) );

    static std::list<std::string> baseline = {"[", "asin", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asin - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = asin(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acos" ) {
    coek::Expression e = acos(v);
    v.value(1);
    REQUIRE( e.value() == Approx(0.0) );
    v.value(0);
    REQUIRE( e.value() == Approx(PI/2.0) );

    static std::list<std::string> baseline = {"[", "acos", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acos - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = acos(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atan" ) {
    coek::Expression e = atan(v);
    v.value(0);
    REQUIRE( e.value() == Approx(0.0) );
    v.value(1);
    REQUIRE( e.value() == Approx(PI/4.0) );

    static std::list<std::string> baseline = {"[", "atan", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atan - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = atan(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sinh" ) {
    coek::Expression e = sinh(v);
    v.value(0);
    REQUIRE( e.value() == Approx(0.0) );
    v.value(1);
    REQUIRE( e.value() == Approx((E-1/E)/2.0) );

    static std::list<std::string> baseline = {"[", "sinh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sinh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = sinh(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cosh" ) {
    coek::Expression e = cosh(v);
    v.value(0);
    REQUIRE( e.value() == Approx(1.0) );
    v.value(1);
    REQUIRE( e.value() == Approx((E+1/E)/2.0) );

    static std::list<std::string> baseline = {"[", "cosh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cosh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = cosh(constant);
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"1.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tanh" ) {
    coek::Expression e = tanh(v);
    v.value(0);
    REQUIRE( e.value() == Approx(0.0) );
    v.value(1);
    REQUIRE( e.value() == Approx((E-1/E)/(E+1/E)) );

    static std::list<std::string> baseline = {"[", "tanh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tanh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = tanh(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asinh" ) {
    coek::Expression e = asinh(v);
    v.value(0);
    REQUIRE( e.value() == Approx(0.0) );
    v.value( (E-1/E)/2.0 );
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "asinh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asinh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = asinh(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acosh" ) {
    coek::Expression e = acosh(v);
    v.value(1);
    REQUIRE( e.value() == Approx(0.0) );
    v.value( (E+1/E)/2.0 );
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "acosh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acosh - constant" ) {
    coek::Expression constant(1);
    coek::Expression e = acosh(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atanh" ) {
    coek::Expression e = atanh(v);
    v.value(0);
    REQUIRE( e.value() == Approx(0.0) );
    v.value( (E-1/E)/(E+1/E) );
    REQUIRE( e.value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "atanh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atanh - constant" ) {
    coek::Expression constant(0);
    coek::Expression e = atanh(constant);
    REQUIRE( e.value() == Approx(0.0) );

    static std::list<std::string> baseline = {"0.000"};
    REQUIRE( e.to_list() == baseline );
  }
}

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_constraint", "[smoke]" ) {

{
auto v = coek::variable("v").lower(0).upper(1).value(1);
auto p = coek::parameter("p");
auto P = coek::set_index("P");
P.value(0);
coek::Expression f;

  SECTION( "lt-operator" ) {

    WHEN( "1 < p" ) {
        coek::Constraint e = 1 < p;
        static std::list<std::string> baseline = {"[", "<", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "p < 1" ) {
        coek::Constraint e = p < 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 < p" ) {
        coek::Constraint e = 1.0 < p;
        static std::list<std::string> baseline = {"[", "<", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "p < 1.0" ) {
        coek::Constraint e = p < 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 < P" ) {
        coek::Constraint e = 1 < P;
        static std::list<std::string> baseline = {"[", "<", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "P < 1" ) {
        coek::Constraint e = P < 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 < P" ) {
        coek::Constraint e = 1.0 < P;
        static std::list<std::string> baseline = {"[", "<", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "P < 1.0" ) {
        coek::Constraint e = P < 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 < v" ) {
        coek::Constraint e = 1 < v;
        static std::list<std::string> baseline = {"[", "<", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "v < 1" ) {
        coek::Constraint e = v < 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "1.0 < v" ) {
        coek::Constraint e = 1.0 < v;
        static std::list<std::string> baseline = {"[", "<", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "v < 1.0" ) {
        coek::Constraint e = v < 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "1 < f" ) {
        coek::Constraint e = 1 < f;
        static std::list<std::string> baseline = {"[", "<", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < 1" ) {
        coek::Constraint e = f < 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 < f" ) {
        coek::Constraint e = 1.0 < f;
        static std::list<std::string> baseline = {"[", "<", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < 1.0" ) {
        coek::Constraint e = f < 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    // TODO - Error because no variables?
    WHEN( "p < p" ) {
        coek::Constraint e = p < p;
        static std::list<std::string> baseline = {"[", "<", "p", "p", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p < P" ) {
        coek::Constraint e = p < P;
        static std::list<std::string> baseline = {"[", "<", "p", "P", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P < p" ) {
        coek::Constraint e = P < p;
        static std::list<std::string> baseline = {"[", "<", "P", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p < v" ) {
        coek::Constraint e = p < v;
        static std::list<std::string> baseline = {"[", "<", "p", "v", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v < p" ) {
        coek::Constraint e = v < p;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p < f" ) {
        coek::Constraint e = p < f;
        static std::list<std::string> baseline = {"[", "<", "p", "0.000", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < p" ) {
        coek::Constraint e = f < p;
        static std::list<std::string> baseline = {"[", "<", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    // TODO - Error because no variables?
    WHEN( "P < P" ) {
        coek::Constraint e = P < P;
        static std::list<std::string> baseline = {"[", "<", "P", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P < v" ) {
        coek::Constraint e = P < v;
        static std::list<std::string> baseline = {"[", "<", "P", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v < P" ) {
        coek::Constraint e = v < P;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P < f" ) {
        coek::Constraint e = P < f;
        static std::list<std::string> baseline = {"[", "<", "P", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < P" ) {
        coek::Constraint e = f < P;
        static std::list<std::string> baseline = {"[", "<", "0.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < v" ) {
        coek::Constraint e = f < v;
        static std::list<std::string> baseline = {"[", "<", "0.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v < f" ) {
        coek::Constraint e = v < f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v < v" ) {
        coek::Constraint e = v < v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f < f" ) {
        coek::Constraint e = f < f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "le-operator" ) {

    WHEN( "0 <= p" ) {
        coek::Constraint e = 0 <= p;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "p <= 0" ) {
        coek::Constraint e = p <= 0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "0.0 <= p" ) {
        coek::Constraint e = 0.0 <= p;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "p <= 0.0" ) {
        coek::Constraint e = p <= 0.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 <= p" ) {
        coek::Constraint e = 1 <= p;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "p <= 1" ) {
        coek::Constraint e = p <= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 <= p" ) {
        coek::Constraint e = 1.0 <= p;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "p <= 1.0" ) {
        coek::Constraint e = p <= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 <= P" ) {
        coek::Constraint e = 1 <= P;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "P <= 1" ) {
        coek::Constraint e = P <= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 <= P" ) {
        coek::Constraint e = 1.0 <= P;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == false );
    }

    WHEN( "P <= 1.0" ) {
        coek::Constraint e = P <= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 <= v" ) {
        coek::Constraint e = 1 <= v;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "v <= 1" ) {
        coek::Constraint e = v <= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1.0 <= v" ) {
        coek::Constraint e = 1.0 <= v;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "v <= 1.0" ) {
        coek::Constraint e = v <= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.is_feasible() == true );
    }

    WHEN( "1 <= f" ) {
        coek::Constraint e = 1 <= f;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= 1" ) {
        coek::Constraint e = f <= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 <= f" ) {
        coek::Constraint e = 1.0 <= f;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= 1.0" ) {
        coek::Constraint e = f <= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p <= p" ) {
        coek::Constraint e = p <= p;
        static std::list<std::string> baseline = {"[", "<=", "p", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p <= P" ) {
        coek::Constraint e = p <= P;
        static std::list<std::string> baseline = {"[", "<=", "p", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P <= p" ) {
        coek::Constraint e = P <= p;
        static std::list<std::string> baseline = {"[", "<=", "P", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p <= v" ) {
        coek::Constraint e = p <= v;
        static std::list<std::string> baseline = {"[", "<=", "p", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v <= p" ) {
        coek::Constraint e = v <= p;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p <= f" ) {
        coek::Constraint e = p <= f;
        static std::list<std::string> baseline = {"[", "<=", "p", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= p" ) {
        coek::Constraint e = f <= p;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P <= P" ) {
        coek::Constraint e = P <= P;
        static std::list<std::string> baseline = {"[", "<=", "P", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P <= v" ) {
        coek::Constraint e = P <= v;
        static std::list<std::string> baseline = {"[", "<=", "P", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v <= P" ) {
        coek::Constraint e = v <= P;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P <= f" ) {
        coek::Constraint e = P <= f;
        static std::list<std::string> baseline = {"[", "<=", "P", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= P" ) {
        coek::Constraint e = f <= P;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= v" ) {
        coek::Constraint e = f <= v;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v <= f" ) {
        coek::Constraint e = v <= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v <= v" ) {
        coek::Constraint e = v <= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f <= f" ) {
        coek::Constraint e = f <= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "eq-operator" ) {

    WHEN( "0 == p" ) {
        coek::Constraint e = 0 == p;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == 0" ) {
        coek::Constraint e = p == 0;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "0.0 == p" ) {
        coek::Constraint e = 0.0 == p;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == 0.0" ) {
        coek::Constraint e = p == 0.0;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 == p" ) {
        coek::Constraint e = 1 == p;
        static std::list<std::string> baseline = {"[", "==", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == 1" ) {
        coek::Constraint e = p == 1;
        static std::list<std::string> baseline = {"[", "==", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 == p" ) {
        coek::Constraint e = 1.0 == p;
        static std::list<std::string> baseline = {"[", "==", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == 1.0" ) {
        coek::Constraint e = p == 1.0;
        static std::list<std::string> baseline = {"[", "==", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 == P" ) {
        coek::Constraint e = 1 == P;
        static std::list<std::string> baseline = {"[", "==", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == 1" ) {
        coek::Constraint e = P == 1;
        static std::list<std::string> baseline = {"[", "==", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 == P" ) {
        coek::Constraint e = 1.0 == P;
        static std::list<std::string> baseline = {"[", "==", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == 1.0" ) {
        coek::Constraint e = P == 1.0;
        static std::list<std::string> baseline = {"[", "==", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 == v" ) {
        coek::Constraint e = 1 == v;
        static std::list<std::string> baseline = {"[", "==", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == 1" ) {
        coek::Constraint e = v == 1;
        static std::list<std::string> baseline = {"[", "==", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 == v" ) {
        coek::Constraint e = 1.0 == v;
        static std::list<std::string> baseline = {"[", "==", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == 1.0" ) {
        coek::Constraint e = v == 1.0;
        static std::list<std::string> baseline = {"[", "==", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 == f" ) {
        coek::Constraint e = 1 == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == 1" ) {
        coek::Constraint e = f == 1;
        static std::list<std::string> baseline = {"[", "==", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 == f" ) {
        coek::Constraint e = 1.0 == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == 1.0" ) {
        coek::Constraint e = f == 1.0;
        static std::list<std::string> baseline = {"[", "==", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == p" ) {
        coek::Constraint e = p == p;
        static std::list<std::string> baseline = {"[", "==", "p", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == P" ) {
        coek::Constraint e = p == P;
        static std::list<std::string> baseline = {"[", "==", "P", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == p" ) {
        coek::Constraint e = P == p;
        static std::list<std::string> baseline = {"[", "==", "p", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == v" ) {
        coek::Constraint e = p == v;
        static std::list<std::string> baseline = {"[", "==", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == p" ) {
        coek::Constraint e = v == p;
        static std::list<std::string> baseline = {"[", "==", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p == f" ) {
        coek::Constraint e = p == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == p" ) {
        coek::Constraint e = f == p;
        static std::list<std::string> baseline = {"[", "==", "p", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == P" ) {
        coek::Constraint e = P == P;
        static std::list<std::string> baseline = {"[", "==", "P", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == v" ) {
        coek::Constraint e = P == v;
        static std::list<std::string> baseline = {"[", "==", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == P" ) {
        coek::Constraint e = v == P;
        static std::list<std::string> baseline = {"[", "==", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P == f" ) {
        coek::Constraint e = P == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == P" ) {
        coek::Constraint e = f == P;
        static std::list<std::string> baseline = {"[", "==", "P", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == v" ) {
        coek::Constraint e = f == v;
        static std::list<std::string> baseline = {"[", "==", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == f" ) {
        coek::Constraint e = v == f;
        static std::list<std::string> baseline = {"[", "==", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v == v" ) {
        coek::Constraint e = v == v;
        static std::list<std::string> baseline = {"[", "==", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f == f" ) {
        coek::Constraint e = f == f;
        static std::list<std::string> baseline = {"[", "==", "0.000", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "gt-operator" ) {

    WHEN( "1 > p" ) {
        coek::Constraint e = 1 > p;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > 1" ) {
        coek::Constraint e = p > 1;
        static std::list<std::string> baseline = {"[", "<", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > p" ) {
        coek::Constraint e = 1.0 > p;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > 1.0" ) {
        coek::Constraint e = p > 1.0;
        static std::list<std::string> baseline = {"[", "<", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 > P" ) {
        coek::Constraint e = 1 > P;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > 1" ) {
        coek::Constraint e = P > 1;
        static std::list<std::string> baseline = {"[", "<", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > P" ) {
        coek::Constraint e = 1.0 > P;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > 1.0" ) {
        coek::Constraint e = P > 1.0;
        static std::list<std::string> baseline = {"[", "<", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 > v" ) {
        coek::Constraint e = 1 > v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > 1" ) {
        coek::Constraint e = v > 1;
        static std::list<std::string> baseline = {"[", "<", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > v" ) {
        coek::Constraint e = 1.0 > v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > 1.0" ) {
        coek::Constraint e = v > 1.0;
        static std::list<std::string> baseline = {"[", "<", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 > f" ) {
        coek::Constraint e = 1 > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > 1" ) {
        coek::Constraint e = f > 1;
        static std::list<std::string> baseline = {"[", "<", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > f" ) {
        coek::Constraint e = 1.0 > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > 1.0" ) {
        coek::Constraint e = f > 1.0;
        static std::list<std::string> baseline = {"[", "<", "1.000", "0.000", "Inf","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > p" ) {
        coek::Constraint e = p > p;
        static std::list<std::string> baseline = { "[", "<", "p", "p", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > P" ) {
        coek::Constraint e = p > P;
        static std::list<std::string> baseline = { "[", "<", "P", "p", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > p" ) {
        coek::Constraint e = P > p;
        static std::list<std::string> baseline = { "[", "<", "p", "P", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > v" ) {
        coek::Constraint e = p > v;
        static std::list<std::string> baseline = { "[", "<", "-Inf", "v", "p", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > p" ) {
        coek::Constraint e = v > p;
        static std::list<std::string> baseline = { "[", "<", "p", "v", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > f" ) {
        coek::Constraint e = p > f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > p" ) {
        coek::Constraint e = f > p;
        static std::list<std::string> baseline = {"[", "<", "p", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > P" ) {
        coek::Constraint e = P > P;
        static std::list<std::string> baseline = { "[", "<", "P", "P", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > v" ) {
        coek::Constraint e = P > v;
        static std::list<std::string> baseline = { "[", "<", "-Inf", "v", "P", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > P" ) {
        coek::Constraint e = v > P;
        static std::list<std::string> baseline = { "[", "<", "P", "v", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > f" ) {
        coek::Constraint e = P > f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > P" ) {
        coek::Constraint e = f > P;
        static std::list<std::string> baseline = {"[", "<", "P", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > v" ) {
        coek::Constraint e = f > v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > f" ) {
        coek::Constraint e = v > f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v > v" ) {
        coek::Constraint e = v > v;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > f" ) {
        coek::Constraint e = f > f;
        static std::list<std::string> baseline = {"[", "<", "0.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "ge-operator" ) {

    WHEN( "1 >= p" ) {
        coek::Constraint e = 1 >= p;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= 1" ) {
        coek::Constraint e = p >= 1;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= p" ) {
        coek::Constraint e = 1.0 >= p;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "p", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= 1.0" ) {
        coek::Constraint e = p >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 >= P" ) {
        coek::Constraint e = 1 >= P;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= 1" ) {
        coek::Constraint e = P >= 1;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= P" ) {
        coek::Constraint e = 1.0 >= P;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "P", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= 1.0" ) {
        coek::Constraint e = P >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 >= v" ) {
        coek::Constraint e = 1 >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= 1" ) {
        coek::Constraint e = v >= 1;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= v" ) {
        coek::Constraint e = 1.0 >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= 1.0" ) {
        coek::Constraint e = v >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1 >= f" ) {
        coek::Constraint e = 1 >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= 1" ) {
        coek::Constraint e = f >= 1;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= f" ) {
        coek::Constraint e = 1.0 >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "0.000", "1.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= 1.0" ) {
        coek::Constraint e = f >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "1.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= p" ) {
        coek::Constraint e = p >= p;
        static std::list<std::string> baseline = { "[", "<=", "p", "p", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= P" ) {
        coek::Constraint e = p >= P;
        static std::list<std::string> baseline = {"[", "<=", "P", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= p" ) {
        coek::Constraint e = P >= p;
        static std::list<std::string> baseline = {"[", "<=", "p", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= v" ) {
        coek::Constraint e = p >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "p", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= p" ) {
        coek::Constraint e = v >= p;
        static std::list<std::string> baseline = {"[", "<=", "p", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= f" ) {
        coek::Constraint e = p >= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "p", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= p" ) {
        coek::Constraint e = f >= p;
        static std::list<std::string> baseline = {"[", "<=", "p", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= P" ) {
        coek::Constraint e = P >= P;
        static std::list<std::string> baseline = { "[", "<=", "P", "P", "Inf", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= v" ) {
        coek::Constraint e = P >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "P", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= P" ) {
        coek::Constraint e = v >= P;
        static std::list<std::string> baseline = {"[", "<=", "P", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= f" ) {
        coek::Constraint e = P >= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "P", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= P" ) {
        coek::Constraint e = f >= P;
        static std::list<std::string> baseline = {"[", "<=", "P", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= v" ) {
        coek::Constraint e = f >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= f" ) {
        coek::Constraint e = v >= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "v", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v >= v" ) {
        coek::Constraint e = v >= v;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "v", "[", "*", "-1", "v", "]", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= f" ) {
        coek::Constraint e = f >= f;
        static std::list<std::string> baseline = {"[", "<=", "0.000", "0.000", "Inf", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "constructor" ) {
    WHEN( "equal" ) {
        coek::Constraint e = v <= 0;
        coek::Constraint f = v >= 0;
        f = e;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test inequality" ) {

    WHEN( "inline" ) {
        auto v = coek::variable("v").lower(0).upper(1).value(0);
        REQUIRE( (v - 1 == 0).is_inequality() == false );
        REQUIRE( (v - 1 == 0).is_equality() == true );
    }

    WHEN( "v <= 0" ) {
      coek::Constraint e = v <= 0;

      static std::list<std::string> baseline = {"[", "<=", "-Inf", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v < 0" ) {
      coek::Constraint e = v < 0;

      static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v < 0.0" ) {
      coek::Constraint e = v < 0.0;

      static std::list<std::string> baseline = {"[", "<", "-Inf", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "-1 < v < 0" ) {
      coek::Constraint e = inequality(-1, v, 0, true);

      static std::list<std::string> baseline = {"[", "<", "-1.000", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "-1 <= v <= 0" ) {
      coek::Constraint e = inequality(-1, v, 0);

      static std::list<std::string> baseline = {"[", "<=", "-1.000", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "-1.0 < v < 0.0" ) {
      coek::Constraint e = inequality(-1.0, v, 0.0, true);

      static std::list<std::string> baseline = {"[", "<", "-1.000", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "-1.0 <= v <= 0.0" ) {
      coek::Constraint e = inequality(-1.0, v, 0.0);

      static std::list<std::string> baseline = {"[", "<=", "-1.000", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v + p <= 0" ) {
      coek::Constraint e = v + p <= 0;

      static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "v", "p", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v + p < 0" ) {
      coek::Constraint e = v + p < 0;

      static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "v", "p", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v + P <= 0" ) {
      coek::Constraint e = v + P <= 0;

      static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "v", "P", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    WHEN( "v + P < 0" ) {
      coek::Constraint e = v + P < 0;

      static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "v", "P", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == true );
      REQUIRE( e.is_equality() == false );
      }

    }

  SECTION( "Test Equality" ) {

    WHEN( "v == 0" ) {
      coek::Constraint e = v == 0;

      static std::list<std::string> baseline = {"[", "==", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == false );
      REQUIRE( e.is_equality() == true );
      }

    WHEN( "v == 0.0" ) {
      coek::Constraint e = v == 0.0;

      static std::list<std::string> baseline = {"[", "==", "v", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == false );
      REQUIRE( e.is_equality() == true );
      }

    WHEN( "v + p == 0" ) {
      coek::Constraint e = v + p == 0;

      static std::list<std::string> baseline = {"[", "==", "[", "+", "v", "p", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == false );
      REQUIRE( e.is_equality() == true );
      }

    WHEN( "v + P == 0" ) {
      coek::Constraint e = v + P == 0;

      static std::list<std::string> baseline = {"[", "==", "[", "+", "v", "P", "]", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.is_inequality() == false );
      REQUIRE( e.is_equality() == true );
      }

    }

  SECTION( "collect_terms" ) {
    coek::QuadraticExpr repn;
    coek::Model m;
    auto v = m.add_variable("v").lower(0).upper(1).value(0);
    coek::Constraint a = 0 >= 1 + v;

    repn.collect_terms(a);
    std::stringstream sstr;
    sstr << repn;
    REQUIRE( sstr.str() == "Constant: 1\nLinear: \n1 v\nQuadratic: \n");
  }

  SECTION( "Constraint Unit Tests" ) {

/*
    WHEN( "Empty" ) {
        coek::Constraint c;
        REQUIRE( c.id() == 0 );
    }
*/
  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "expression_value", "[smoke]" ) {

{
auto a = coek::variable("a").lower(0.0).upper(1.0).value(0.0);
auto b = coek::variable("b").lower(0.0).upper(1.0).value(1.0);
auto q = coek::parameter("q").value(2);

  SECTION( "expression" ) {

    WHEN( "e = q" ) {
        coek::Expression e = q;
        REQUIRE( e.value() == 2 );
    }

    WHEN( "e = a" ) {
        coek::Expression e = a;
        REQUIRE( e.value() == 0 );
    }

    WHEN( "e = 3*b + q" ) {
        coek::Expression e = 3*b + q;
        REQUIRE( e.value() == 5.0 );
    }

  }

  SECTION( "constraint" ) {

    WHEN( "e = 3*b + q == 0" ) {
        coek::Constraint e = 3*b + q == 0;
        REQUIRE( e.get_body().value() == 5.0 );
        REQUIRE( e.is_feasible() == false );
    }
  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}
