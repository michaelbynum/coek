
#include <cmath>
#include <memory>
#include <sstream>
#include <iostream>
#include "catch.hpp"

#include "coek/ast/base_terms.hpp"
#include "coek/ast/value_terms.hpp"
#include "coek/coek.hpp"

const double PI = 3.141592653589793238463;
const double E = exp(1.0);


/*
TEST_CASE( "capi_misc", "[smoke]" ) {

  SECTION( "Misc API functions" ) {
    REQUIRE( misc_getnull() == 0 );
    REQUIRE( std::isnan(misc_getnan()) );
  }

  SECTION( "API printing" ) {
      std::stringstream os;
      std::streambuf* coutbuf = std::cout.rdbuf();
      std::cout.rdbuf(os.rdbuf());

      WHEN( "variable" ) {
        coek::Variable a(0.0, 1.0, 0.0, false, false, "a");
        std::cout << a << std::endl;

        std::string tmp = os.str();
        REQUIRE( tmp == "a{0}\n" );

        char ctmp[10];
        get_numval_str(a, ctmp, 10);
        tmp = ctmp;
        REQUIRE( tmp == "a{0.000}" );
      }

      WHEN( "parameter - int" ) {
        apival_t q = create_parameter_int(0, 2, true, "q");
        print_parameter(q);

        std::string tmp = os.str();
        REQUIRE( tmp == "q{2}\n" );

        char ctmp[10];
        get_numval_str(q, ctmp, 10);
        tmp = ctmp;
        REQUIRE( tmp == "q{2}" );
      }

      WHEN( "parameter - double" ) {
        apival_t q = create_parameter_double(0, 2.0, true, "q");
        print_parameter(q);

        std::string tmp = os.str();
        REQUIRE( tmp == "q{2}\n" );

        char ctmp[10];
        get_numval_str(q, ctmp, 10);
        tmp = ctmp;
        REQUIRE( tmp == "q{2.000}" );
      }

      WHEN( "expression" ) {
        apival_t a = create_variable(0, false, false, 0.0, 1.0, 0.0, "a");
        Variable* v = static_cast<Variable*>(a);
        v->index = 1;
        apival_t q = create_parameter_int(0, 0, true, "q");
        apival_t e = expr_plus_expression(a,q);
        print_expr(e);

        std::string tmp = os.str();
        REQUIRE( tmp == "a{0} + q{0}\n" );
      }

      std::cout.rdbuf(coutbuf);
  }

  SECTION( "API expressions" ) {
      WHEN( "size" ) {
        apival_t a = create_variable(0, false, false, 0.0, 1.0, 0.0, "a");
        apival_t q = create_parameter_int(0, 0, true, "q");
        apival_t e = expr_plus_expression(a,q);

        REQUIRE( expr_size(e) == 3 );
      }
  }
}
*/


TEST_CASE( "model_parameter", "[smoke]" ) {

  SECTION( "values" ) {
      WHEN( "parameter - 3" ) {
        coek::Parameter q(2, "q");
        REQUIRE( q.get_value() == 2 );
        q.set_value(3);
        REQUIRE( q.get_value() == 3 );
      }
  }

  SECTION( "constructors" ) {
      WHEN( "copy" ) {
        // Simple constructor
        coek::Parameter q(3);

        // Test copy constructor
        coek::Parameter Q(q);
        REQUIRE( Q.get_value() == 3 );
      }

      WHEN( "equal" ) {
        // Simple constructor
        coek::Parameter q(4);

        // Test copy constructor
        coek::Parameter Q(5);
        Q = q;
        REQUIRE( Q.get_value() == 4 );
      }
  }

  SECTION( "constructors" ) {
    coek::Parameter q(3, "q");
    REQUIRE( q.get_name() == "q" );
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    coek::Parameter q(3, "q");
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}

TEST_CASE( "model_variables", "[smoke]" ) {

  SECTION( "constructors" ) {
    WHEN( "simple" ) {
        coek::Variable a(0,1,2);
        REQUIRE( a.get_value() == 2 );
        REQUIRE( a.get_lb() == 0 );
        REQUIRE( a.get_ub() == 1 );
        // Keep this test?  The integer index depends on whether this test is run first.
        REQUIRE( a.get_name() == "x(0)" );
    }

    WHEN( "named" ) {
        coek::Variable a("test",0,1,2);
        REQUIRE( a.get_value() == 2 );
        REQUIRE( a.get_lb() == 0 );
        REQUIRE( a.get_ub() == 1 );
        REQUIRE( a.get_name() == "test" );
    }

    WHEN( "copy" ) {
        coek::Variable a("test",0,1,2);
        coek::Variable b(a);
        REQUIRE( b.get_value() == 2 );
        REQUIRE( b.get_lb() == 0 );
        REQUIRE( b.get_ub() == 1 );
        REQUIRE( b.get_name() == "test" );
        REQUIRE( a.id() == b.id() );
    }

    WHEN( "equal" ) {
        coek::Variable a("test",0,1,2);
        coek::Variable b;
        b = a;
        REQUIRE( b.get_value() == 2 );
        REQUIRE( b.get_lb() == 0 );
        REQUIRE( b.get_ub() == 1 );
        REQUIRE( b.get_name() == "test" );
        REQUIRE( a.id() == b.id() );
    }
  }

  SECTION( "index" ) {
    coek::Model model;
    coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
    model.add_variable(a);
    coek::Variable b = model.add_variable("b",0,1,0,false,false); 
    REQUIRE( a.id() == (b.id() - 1) );
  }

  SECTION( "values" ) {
      WHEN( "variable - 3" ) {
        coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
        REQUIRE( a.get_value() == 0 );
        a.set_value(3);
        REQUIRE( a.get_value() == 3 );
      }

/*
      WHEN( "variable array - 3" ) {
        void* a[2];
        create_variable_array(0, a, 2, false, false, 0.0, 1.0, 0.0, "a");
        variable_set_value(a[0], 3);

        REQUIRE( variable_get_value(a[0]) == 3 );
        REQUIRE( variable_get_value(a[1]) == 0 );
      }

      WHEN( "variable array (2) - 3" ) {
        apival_t model = create_model();

        void* a[2];
        create_variable_array(model, a, 2, false, false, 0.0, 1.0, 0.0, "a");
        variable_set_value(a[0], 3);

        REQUIRE( variable_get_value(a[0]) == 3 );
        REQUIRE( variable_get_value(a[1]) == 0 );
      }
*/
  }

  SECTION( "bounds" ) {
      WHEN( "lb" ) {
        coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
        REQUIRE( a.get_lb() == 0.0 );
        a.set_lb(3.0);
        REQUIRE( a.get_lb() == 3.0 );
      }

      WHEN( "ub" ) {
        coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
        REQUIRE( a.get_ub() == 1.0 );
        a.set_ub(3.0);
        REQUIRE( a.get_ub() == 3.0 );
      }
  }

  SECTION( "properties" ) {
    WHEN( "continuous" ) {
        coek::Variable a;
        REQUIRE( a.is_continuous() == true );
        REQUIRE( a.is_binary() == false );
        REQUIRE( a.is_integer() == false );
    }

    WHEN( "binary" ) {
        coek::Variable a("a", 0,1,0,true,false);
        REQUIRE( a.is_continuous() == false );
        REQUIRE( a.is_binary() == true );
        REQUIRE( a.is_integer() == false );
    }

    WHEN( "integer" ) {
        coek::Variable a("a", 0,10,5,false,true);
        REQUIRE( a.is_continuous() == false );
        REQUIRE( a.is_binary() == false );
        REQUIRE( a.is_integer() == true );
    }
  }

  SECTION( "write" ) {
    std::stringstream sstr;
    coek::Variable q("q", 0,1,0);
    sstr << q;
    REQUIRE( sstr.str() == "q" );
  }

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_monomial", "[smoke]" ) {

  SECTION( "trivial" ) {
    coek::Variable v("v",0,1,0);
    coek::Expression e = 1*v;
    REQUIRE( e.repn == v.repn );
    static std::list<std::string> baseline = {"v"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "simple monomial" ) {
    coek::Variable v("v",0,1,0);
    coek::Expression e = 2*v;
    static std::list<std::string> baseline = {"[", "*", "2", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

/*
  SECTION( "unnamed monomial" ) {
    coek::Variable v(0,1,0);
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
        coek::Parameter p(1.0);
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }

    WHEN( "index_parameter" ) {
        coek::IndexParameter p("p");
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }

    WHEN( "variable" ) {
        coek::Variable p(0,1,0);
        coek::Expression a;
        a = p;
        REQUIRE( a.is_constant() == false );
    }
  }

  SECTION( "plus-equal" ) {
    coek::Expression a(1.0);
    coek::Variable p("p",0,1,0);
    a += p;
    static std::list<std::string> baseline = {"[", "+", "1.000", "p", "]"};
    REQUIRE( a.to_list() == baseline );
  }

  SECTION( "minus-equal" ) {
    coek::Expression a(1.0);
    coek::Variable p("p",0,1,0);
    a -= p;
    static std::list<std::string> baseline = {"[", "+", "1.000", "[", "-", "p", "]", "]"};
    REQUIRE( a.to_list() == baseline );
  }

  SECTION( "times-equal" ) {
    coek::Expression a(1.0);
    coek::Variable p("p",0,1,0);
    a *= p;
    static std::list<std::string> baseline = {"[", "*", "1.000", "p", "]"};
    REQUIRE( a.to_list() == baseline );
  }

  SECTION( "divide-equal" ) {
    coek::Expression a(1.0);
    coek::Variable p("p",0,1,0);
    a /= p;
    static std::list<std::string> baseline = {"[", "/", "1.000", "p", "]"};
    REQUIRE( a.to_list() == baseline );
  }

  SECTION( "collect_terms" ) {
    coek::QuadraticExpr repn;
    coek::Model m;
    coek::Variable v = m.add_variable("v",0,0,0,false,false);
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


/*
TEST_CASE( "capi_errors", "[smoke]" ) {
    SECTION( "parameter" ) {
        REQUIRE_THROWS_AS( create_parameter_int(0, 0, true, "q"), std::runtime_error );
        REQUIRE_THROWS_AS( create_parameter_double(0, 0, true, "Q"), std::runtime_error );
        REQUIRE_THROWS_AS( get_parameter_zero(0), std::runtime_error );
        REQUIRE_THROWS_AS( get_parameter_one(0), std::runtime_error );
        REQUIRE_THROWS_AS( get_parameter_negative_one(0), std::runtime_error );
    }

    SECTION( "variable" ) {
        REQUIRE_THROWS_AS( create_variable(0, false, false, 0.0, 1.0, 0.0, "a"), std::runtime_error );
        REQUIRE_THROWS_AS( create_variable_array(0, 0, 0, false, false, 0.0, 1.0, 0.0, "A"), std::runtime_error );
    }
}
*/


TEST_CASE( "model_unary_expression", "[smoke]" ) {

  SECTION( "positive" ) {
    WHEN( "param" ) {
        coek::Parameter q(1.0, "q");
        coek::Expression e;
        e = +q;
        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "iparam" ) {
        coek::IndexParameter q("q");
        coek::Expression e;
        e = +q;
        static std::list<std::string> baseline = {"q"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "var" ) {
        coek::Variable v("v",0,1,0);
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
        coek::Parameter q(1.0, "q");
        coek::Expression e;
        e = -q;
        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.get_value() == -1 );
    }

    WHEN( "iparam" ) {
        coek::IndexParameter q("q");
        coek::Expression e;
        e = -q;
        static std::list<std::string> baseline = {"[", "-", "q", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "var" ) {
        coek::Variable v("v",0,1,2);
        coek::Expression e;
        e = -v;
        static std::list<std::string> baseline = {"[", "*", "-1", "v", "]"};
        REQUIRE( e.to_list() == baseline );
        REQUIRE( e.get_value() == -2 );
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
coek::Variable a("a",0.0, 1.0, 3.0, false, false);
coek::Variable b("b",0.0, 1.0, 5.0, false, false);
coek::Variable c("c", 0.0, 1.0, 0.0, false, false);
coek::Variable d("d",0.0, 1.0, 0.0, false, false);
coek::Parameter z(0.0, "z");
coek::IndexParameter Z("Z");

  SECTION( "Test simpleSum" ) {
    coek::Expression e = a + b;

    static std::list<std::string> baseline = {"[", "+", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.get_value() == 8 );
  }

  SECTION( "Test simpleSum_API" ) {
    coek::Expression e1 = a + b;
    coek::Expression e2 = 2 * a;
    coek::Expression e = e1 + e2;

    static std::list<std::string> baseline = { "[", "+", "a", "b", "[", "*", "2", "a", "]", "]" };
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test constSum" ) {
    WHEN( "e = a + 5" ) {
        coek::Expression e = a + 5;

        static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + a" ) {
        coek::Expression e = 5 + a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 5.0" ) {
        coek::Expression e = a + 5.0;

        static std::list<std::string> baseline = {"[", "+", "a", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + a" ) {
        coek::Expression e = 5.0 + a;

        static std::list<std::string> baseline = {"[", "+", "5.000", "a", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + 5" ) {
        coek::Expression e = z + 5;

        static std::list<std::string> baseline = {"[", "+", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + 5" ) {
        coek::Expression e = Z + 5;

        static std::list<std::string> baseline = {"[", "+", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + z" ) {
        coek::Expression e = 5 + z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5 + Z" ) {
        coek::Expression e = 5 + Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + 5.0" ) {
        coek::Expression e = z + 5.0;

        static std::list<std::string> baseline = {"[", "+", "z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + 5.0" ) {
        coek::Expression e = Z + 5.0;

        static std::list<std::string> baseline = {"[", "+", "Z", "5.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + z" ) {
        coek::Expression e = 5.0 + z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0 + Z" ) {
        coek::Expression e = 5.0 + Z;

        static std::list<std::string> baseline = {"[", "+", "5.000", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z + Z" ) {
        coek::Expression e = z + Z;

        static std::list<std::string> baseline = {"[", "+", "z", "Z", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z + z" ) {
        coek::Expression e = Z + z;

        static std::list<std::string> baseline = {"[", "+", "Z", "z", "]"};
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
    coek::Parameter q(0, "q");
    coek::Parameter Q(0, "Q");

    WHEN( "e = a + q{0}" ) {
        coek::Expression e = a + q;

        static std::list<std::string> baseline = {"[","+","a","q","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 0" ) {
        coek::Expression e = a + 0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q{0} + a" ) {
        coek::Expression e = q + a;

        static std::list<std::string> baseline = {"[","+","q","a","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 + a" ) {
        coek::Expression e = 0 + a;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + Q{0.0}" ) {
        coek::Expression e = a + Q;

        static std::list<std::string> baseline = {"[","+","a","Q","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a + 0.0" ) {
        coek::Expression e = a + 0.0;

        static std::list<std::string> baseline = {"a"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} + a" ) {
        coek::Expression e = Q + a;

        static std::list<std::string> baseline = {"[","+","Q","a","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 + a" ) {
        coek::Expression e = 0.0 + a;

        static std::list<std::string> baseline = {"a"};
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

  SECTION( "Test trivialSum" ) {
    coek::Variable v("v",0,1,1);
    coek::Variable w("w",0,1,1);
    coek::Parameter p(1,"p");
    coek::Parameter q(1,"q");

    WHEN( "p + q" ) {
        coek::Expression e = p + q;

        static std::list<std::string> baseline = { "[", "+", "p", "q", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p + v" ) {
        coek::Expression e = p + v;

        static std::list<std::string> baseline = { "[", "+", "p", "v", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "w + q" ) {
        coek::Expression e = w + q;

        static std::list<std::string> baseline = { "[", "+", "w", "q", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "w + v" ) {
        coek::Expression e = w + v;

        static std::list<std::string> baseline = { "[", "+", "w", "v", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e + q" ) {
        coek::Expression f;
        coek::Expression e = f + q;

        static std::list<std::string> baseline = { "[", "+", "0.000", "q", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e + v" ) {
        coek::Expression f;
        coek::Expression e = f + v;

        static std::list<std::string> baseline = { "[", "+", "0.000", "v", "]" };
    }

    WHEN( "q + e" ) {
        coek::Expression f;
        coek::Expression e = q + f;

        static std::list<std::string> baseline = { "[", "+", "q", "0.000", "]" };
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "v + e" ) {
        coek::Expression f;
        coek::Expression e = v + f;

        static std::list<std::string> baseline = { "[", "+", "v", "0.000", "]" };
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
coek::Variable a("a",0.0, 1.0, 3.0, false, false);
coek::Variable b("b",0.0, 1.0, 5.0, false, false);
coek::Variable c("c",0.0, 1.0, 0.0, false, false);
coek::Variable d("d",0.0, 1.0, 0.0, false, false);

coek::Parameter z(1.0, "z");
coek::IndexParameter Z("Z");
coek::Expression f;

  SECTION( "Test simpleDiff" ) {
    coek::Expression e = a - b;

    static std::list<std::string> baseline = { "[", "+", "a", "[", "*", "-1", "b", "]", "]" };
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.get_value() == -2 );
  }

  SECTION( "Test constDiff" ) {

    WHEN( "e = a - 5" ) {
    coek::Expression e = a - 5;

      static std::list<std::string> baseline = {"[", "+", "a", "-5.000", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = 5 - a" ) {
      coek::Expression e = 5 - a;

      static std::list<std::string> baseline = { "[", "+", "5.000", "[", "*", "-1", "a", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = a - 5.0" ) {
      coek::Expression e = a - 5.0;

      static std::list<std::string> baseline = {"[", "+", "a", "-5.000", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = 5.0 - a" ) {
      coek::Expression e = 5.0 - a;

      static std::list<std::string> baseline = { "[", "+", "5.000", "[", "*", "-1", "a", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = z - 5" ) {
    coek::Expression e = z - 5;

      static std::list<std::string> baseline = {"[", "+", "z", "-5.000", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = Z - 5" ) {
    coek::Expression e = Z - 5;

      static std::list<std::string> baseline = {"[", "+", "Z", "-5.000", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = 5 - z" ) {
      coek::Expression e = 5 - z;

      static std::list<std::string> baseline = { "[", "+", "5.000", "[", "-", "z", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = 5 - Z" ) {
      coek::Expression e = 5 - Z;

      static std::list<std::string> baseline = { "[", "+", "5.000", "[", "-", "Z", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = z - 5.0" ) {
      coek::Expression e = z - 5.0;

      static std::list<std::string> baseline = {"[", "+", "z", "-5.000", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = Z - 5.0" ) {
      coek::Expression e = Z - 5.0;

      static std::list<std::string> baseline = {"[", "+", "Z", "-5.000", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = 5.0 - z" ) {
      coek::Expression e = 5.0 - z;

      static std::list<std::string> baseline = { "[", "+", "5.000", "[", "-", "z", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = 5.0 - Z" ) {
      coek::Expression e = 5.0 - Z;

      static std::list<std::string> baseline = { "[", "+", "5.000", "[", "-", "Z", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = f - 5" ) {
    coek::Expression e = f - 5;

      static std::list<std::string> baseline = {"[", "+", "0.000", "-5.000", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = 5 - f" ) {
      coek::Expression e = 5 - f;

      static std::list<std::string> baseline = { "[", "+", "5.000", "-0.000", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = f - 5.0" ) {
      coek::Expression e = f - 5.0;

      static std::list<std::string> baseline = {"[", "+", "0.000", "-5.000", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = 5.0 - f" ) {
      coek::Expression e = 5.0 - f;

      static std::list<std::string> baseline = { "[", "+", "5.000", "-0.000", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = z - Z" ) {
      coek::Expression e = z - Z;

      static std::list<std::string> baseline = { "[", "+", "z", "[", "-", "Z", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = Z - z" ) {
      coek::Expression e = Z - z;

      static std::list<std::string> baseline = { "[", "+", "Z", "[", "-", "z", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }
  }

  SECTION( "Test paramDiff" ) {
    coek::Parameter p(5, "p");

    WHEN( "e = f - p" ) {
      coek::Expression e = f - p;

      static std::list<std::string> baseline = {"[", "+", "0.000", "[", "-", "p", "]", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = p - f" ) {
      coek::Expression e = p - f;

      static std::list<std::string> baseline = { "[", "+", "p", "-0.000", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = z - p" ) {
      coek::Expression e = z - p;

      static std::list<std::string> baseline = {"[", "+", "z", "[", "-", "p", "]", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = Z - p" ) {
      coek::Expression e = Z - p;

      static std::list<std::string> baseline = {"[", "+", "Z", "[", "-", "p", "]", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = p - z" ) {
      coek::Expression e = p - z;

      static std::list<std::string> baseline = { "[", "+", "p", "[", "-", "z", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = p - Z" ) {
      coek::Expression e = p - Z;

      static std::list<std::string> baseline = { "[", "+", "p", "[", "-", "Z", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = a - p" ) {
      coek::Expression e = a - p;

      static std::list<std::string> baseline = {"[", "+", "a", "[", "-", "p", "]", "]"};
      REQUIRE( e.to_list() == baseline );
      }

    WHEN( "e = p - a" ) {
      coek::Expression e = p - a;

      static std::list<std::string> baseline = { "[", "+", "p", "[", "*", "-1", "a", "]", "]" };
      REQUIRE( e.to_list() == baseline );
      }
  }

  SECTION( "Test termDiff" ) {
    coek::Expression e = 5 - 2*a;

    static std::list<std::string> baseline = { "[", "+", "5.000", "[", "*", "-2", "a", "]", "]" };
    REQUIRE( e.to_list() == baseline );
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
    coek::Parameter q(0, "q");
    coek::Parameter Q(0.0, "Q");

    WHEN( "e = a - q{0}" ) {
      coek::Expression e = a - q;

      static std::list<std::string> baseline = {"[","+","a","[","-","q","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 0*q" ) {
      coek::Expression e = a - 0*q;

      static std::list<std::string> baseline = {"[", "+", "a", "-0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 0" ) {
      coek::Expression e = a - 0;

      static std::list<std::string> baseline = {"a"};
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

    WHEN( "e = 0 - a" ) {
      coek::Expression e = 0 - a;

      static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - Q{0.0}" ) {
      coek::Expression e = a - Q;

      static std::list<std::string> baseline = {"[","+","a","[","-","Q","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - Q_{0.0}" ) {
      coek::Expression e = a - 0.0*Q;

      static std::list<std::string> baseline = {"[", "+", "a", "-0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a - 0.0" ) {
      coek::Expression e = a - 0.0;

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} - a" ) {
      coek::Expression e = Q - a;

      static std::list<std::string> baseline = {"[","+","Q","[","*","-1","a","]","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q_{0.0} - a" ) {
      coek::Expression e = Q*0.0 - a;

      static std::list<std::string> baseline = {"[", "+", "0.000", "[", "*", "-1", "a", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 - a" ) {
      coek::Expression e = 0.0 - a;

      static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
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
    coek::Parameter p(2, "p");

    WHEN( "e = -p" ) {
      coek::Expression e = -p;

      static std::list<std::string> baseline = {"[", "-", "p", "]"};
      REQUIRE( e.to_list() == baseline );
      REQUIRE( e.get_value() == -2 );
    }

    WHEN( "e = -(-p)" ) {
      coek::Expression e = -(-p);

      static std::list<std::string> baseline = {"[", "-", "[", "-", "p", "]", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test negation_terms" ) {
    coek::Parameter p(2, "p");
    coek::Variable v("v",0.0, 1.0, 0.0, false, false);

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
coek::Variable a("a", 0.0, 1.0, 3.0, false, false);
coek::Variable b("b", 0.0, 1.0, 5.0, false, false);
coek::Variable c("c", 0.0, 1.0, 0.0, false, false);
coek::Variable d("d", 0.0, 1.0, 0.0, false, false);

coek::Parameter z(0.0, "z");
coek::IndexParameter Z("Z");
coek::Expression f;

  SECTION( "Test simpleProduct" ) {
    coek::Expression e = a*b;

    static std::list<std::string> baseline = {"[", "*", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.get_value() == 15 );
  }

  SECTION( "Test constProduct" ) {

    WHEN( "e = a*5" ) {
      coek::Expression e = a*5;

      static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*a" ) {
      coek::Expression e = 5*a;

      static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a*5.0" ) {
      coek::Expression e = a*5.0;

      static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0*a" ) {
      coek::Expression e = 5.0*a;

      static std::list<std::string> baseline = {"[", "*", "5", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f*5" ) {
      coek::Expression e = f*5;

      static std::list<std::string> baseline = {"[", "*", "0.000", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*f" ) {
      coek::Expression e = 5*f;

      static std::list<std::string> baseline = {"[", "*", "5.000", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f*5.0" ) {
      coek::Expression e = f*5.0;

      static std::list<std::string> baseline = {"[", "*", "0.000", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0*f" ) {
      coek::Expression e = 5.0*f;

      static std::list<std::string> baseline = {"[", "*", "5.000", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z*5" ) {
      coek::Expression e = z*5;

      static std::list<std::string> baseline = {"[", "*", "z", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z*5" ) {
      coek::Expression e = Z*5;

      static std::list<std::string> baseline = {"[", "*", "Z", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*z" ) {
      coek::Expression e = 5*z;

      static std::list<std::string> baseline = {"[", "*", "5.000", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5*Z" ) {
      coek::Expression e = 5*Z;

      static std::list<std::string> baseline = {"[", "*", "5.000", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z*5.0" ) {
      coek::Expression e = z*5.0;

      static std::list<std::string> baseline = {"[", "*", "z", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z*5.0" ) {
      coek::Expression e = Z*5.0;

      static std::list<std::string> baseline = {"[", "*", "Z", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0*z" ) {
      coek::Expression e = 5.0*z;

      static std::list<std::string> baseline = {"[", "*", "5.000", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0*Z" ) {
      coek::Expression e = 5.0*Z;

      static std::list<std::string> baseline = {"[", "*", "5.000", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z*z" ) {
      coek::Expression e = z*z;

      static std::list<std::string> baseline = {"[", "*", "z", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z*Z" ) {
      coek::Expression e = Z*Z;

      static std::list<std::string> baseline = {"[", "*", "Z", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z*a" ) {
      coek::Expression e = z*a;

      static std::list<std::string> baseline = {"[", "*", "z", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z*a" ) {
      coek::Expression e = Z*a;

      static std::list<std::string> baseline = {"[", "*", "Z", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a*z" ) {
      coek::Expression e = a*z;

      static std::list<std::string> baseline = {"[", "*", "a", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a*Z" ) {
      coek::Expression e = a*Z;

      static std::list<std::string> baseline = {"[", "*", "a", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f*z" ) {
      coek::Expression e = f*z;

      static std::list<std::string> baseline = {"[", "*", "0.000", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f*Z" ) {
      coek::Expression e = f*Z;

      static std::list<std::string> baseline = {"[", "*", "0.000", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z*f" ) {
      coek::Expression e = z*f;

      static std::list<std::string> baseline = {"[", "*", "z", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z*f" ) {
      coek::Expression e = Z*f;

      static std::list<std::string> baseline = {"[", "*", "Z", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f*a" ) {
      coek::Expression e = f*a;

      static std::list<std::string> baseline = {"[", "*", "0.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a*f" ) {
      coek::Expression e = a*f;

      static std::list<std::string> baseline = {"[", "*", "a", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z*Z" ) {
      coek::Expression e = z*Z;

      static std::list<std::string> baseline = {"[", "*", "z", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z*z" ) {
      coek::Expression e = Z*z;

      static std::list<std::string> baseline = {"[", "*", "Z", "z", "]"};
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

  SECTION( "Test trivialProduct" ) {
    coek::Parameter q(0, "q");
    coek::Parameter Q(0.0, "Q");
    coek::Parameter r(1, "r");
    coek::Parameter R(1.0, "R");
    coek::Parameter s(-1, "s");
    coek::Parameter S(-1.0, "S");

    coek::Expression q_(0);
    coek::Expression Q_(0.0);
    coek::Expression r_(1);
    coek::Expression R_(1.0);
    coek::Expression s_(-1);
    coek::Expression S_(-1.0);

    WHEN( "e = a * q{0}" ) {
      coek::Expression e = a * q;

      static std::list<std::string> baseline = {"[","*","a","q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * q_{0}" ) {
      coek::Expression e = a * q_;

      static std::list<std::string> baseline = {"[", "*", "a", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 0" ) {
      coek::Expression e = a * 0;

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = (a+a) * 0" ) {
      coek::Expression e = (a+a) * 0;

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q{0} * a" ) {
      coek::Expression e = q * a;

      static std::list<std::string> baseline = {"[","*","q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q_{0} * a" ) {
      coek::Expression e = q_ * a;

      static std::list<std::string> baseline = {"[", "*", "0.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 * a" ) {
      coek::Expression e = 0 * a;

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * Q{0.0}" ) {
      coek::Expression e = a * Q;

      static std::list<std::string> baseline = {"[","*","a","Q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * Q_{0.0}" ) {
      coek::Expression e = a * Q_;

      static std::list<std::string> baseline = {"[", "*", "a", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 0.0" ) {
      coek::Expression e = a * 0.0;

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} * a" ) {
      coek::Expression e = Q * a;

      static std::list<std::string> baseline = {"[","*","Q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q_{0.0} * a" ) {
      coek::Expression e = Q_ * a;

      static std::list<std::string> baseline = {"[", "*", "0.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 * a" ) {
      coek::Expression e = 0.0 * a;

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * r{1}" ) {
      coek::Expression e = a * r;

      static std::list<std::string> baseline = {"[","*","a","r","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * r_{1}" ) {
      coek::Expression e = a * r_;

      static std::list<std::string> baseline = {"[", "*", "a", "1.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 1" ) {
      coek::Expression e = a * 1;

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = r{1} * a" ) {
      coek::Expression e = r * a;

      static std::list<std::string> baseline = {"[","*","r","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = r_{1} * a" ) {
      coek::Expression e = r_ * a;

      static std::list<std::string> baseline = {"[", "*", "1.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 * a" ) {
      coek::Expression e = 1 * a;

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * R{1.0}" ) {
      coek::Expression e = a * R;

      static std::list<std::string> baseline = {"[","*","a","R","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * R_{1.0}" ) {
      coek::Expression e = a * R_;

      static std::list<std::string> baseline = {"[", "*", "a", "1.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * 1.0" ) {
      coek::Expression e = a * 1.0;

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = R{1.0} * a" ) {
      coek::Expression e = R * a;

      static std::list<std::string> baseline = {"[","*","R","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = R_{1.0} * a" ) {
      coek::Expression e = R_ * a;

      static std::list<std::string> baseline = {"[", "*", "1.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 * a" ) {
      coek::Expression e = 1.0 * a;

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * s{-1}" ) {
      coek::Expression e = a * s;

      static std::list<std::string> baseline = {"[","*","a","s","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * s_{-1}" ) {
      coek::Expression e = a * s_;

      static std::list<std::string> baseline = {"[", "*", "a", "-1.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * -1" ) {
      coek::Expression e = a * -1;

      static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = s{-1} * a" ) {
      coek::Expression e = s * a;

      static std::list<std::string> baseline = {"[","*","s","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = s_{-1} * a" ) {
      coek::Expression e = s_ * a;

      static std::list<std::string> baseline = {"[", "*", "-1.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = -1 * a" ) {
      coek::Expression e = -1 * a;

      static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * S{-1.0}" ) {
      coek::Expression e = a * S;

      static std::list<std::string> baseline = {"[","*","a","S","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * S_{-1.0}" ) {
      coek::Expression e = a * S_;

      static std::list<std::string> baseline = {"[", "*", "a", "-1.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a * -1.0" ) {
      coek::Expression e = a * -1.0;

      static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = S{-1.0} * a" ) {
      coek::Expression e = S * a;

      static std::list<std::string> baseline = {"[","*","S","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = S_{-1.0} * a" ) {
      coek::Expression e = S_ * a;

      static std::list<std::string> baseline = {"[", "*", "-1.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = -1.0 * a" ) {
      coek::Expression e = -1.0 * a;

      static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
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
coek::Variable a("a", 0.0, 1.0, 3.0, false, false);
coek::Variable b("b", 0.0, 1.0, 5.0, false, false);
coek::Variable c("c", 0.0, 1.0, 0.0, false, false);
coek::Variable d("d", 0.0, 1.0, 0.0, false, false);

coek::Parameter z(0.0, "z");
coek::IndexParameter Z("Z");
coek::Expression f;

  SECTION( "Test simpleDivision" ) {
    coek::Expression e = a/b;

    static std::list<std::string> baseline = {"[", "/", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
    REQUIRE( e.get_value() == 0.6 );
  }

  SECTION( "Test constDivision" ) {

    WHEN( "e = a/5" ) {
      coek::Expression e = a/5;

      static std::list<std::string> baseline = {"[", "*", "0.2", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5/a" ) {
      coek::Expression e = 5/a;

      static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/5.0" ) {
      coek::Expression e = a/5.0;

      static std::list<std::string> baseline = {"[", "*", "0.2", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0/a" ) {
      coek::Expression e = 5.0/a;

      static std::list<std::string> baseline = {"[", "/", "5.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f/5" ) {
      coek::Expression e = f/5;

      static std::list<std::string> baseline = {"[", "/", "0.000", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5/f" ) {
      coek::Expression e = 5/f;

      static std::list<std::string> baseline = {"[", "/", "5.000", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f/5.0" ) {
      coek::Expression e = f/5.0;

      static std::list<std::string> baseline = {"[", "/", "0.000", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0/f" ) {
      coek::Expression e = 5.0/f;

      static std::list<std::string> baseline = {"[", "/", "5.000", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z/5" ) {
      coek::Expression e = z/5;

      static std::list<std::string> baseline = {"[", "/", "z", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/5" ) {
      coek::Expression e = Z/5;

      static std::list<std::string> baseline = {"[", "/", "Z", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5/z" ) {
      coek::Expression e = 5/z;

      static std::list<std::string> baseline = {"[", "/", "5.000", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5/Z" ) {
      coek::Expression e = 5/Z;

      static std::list<std::string> baseline = {"[", "/", "5.000", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z/5.0" ) {
      coek::Expression e = z/5.0;

      static std::list<std::string> baseline = {"[", "/", "z", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/5.0" ) {
      coek::Expression e = Z/5.0;

      static std::list<std::string> baseline = {"[", "/", "Z", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0/z" ) {
      coek::Expression e = 5.0/z;

      static std::list<std::string> baseline = {"[", "/", "5.000", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0/Z" ) {
      coek::Expression e = 5.0/Z;

      static std::list<std::string> baseline = {"[", "/", "5.000", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z/z" ) {
      coek::Expression e = z/z;

      static std::list<std::string> baseline = {"[", "/", "z", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/Z" ) {
      coek::Expression e = Z/Z;

      static std::list<std::string> baseline = {"[", "/", "Z", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z/a" ) {
      coek::Expression e = z/a;

      static std::list<std::string> baseline = {"[", "/", "z", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/a" ) {
      coek::Expression e = Z/a;

      static std::list<std::string> baseline = {"[", "/", "Z", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/z" ) {
      coek::Expression e = a/z;

      static std::list<std::string> baseline = {"[", "/", "a", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/Z" ) {
      coek::Expression e = a/Z;

      static std::list<std::string> baseline = {"[", "/", "a", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f/z" ) {
      coek::Expression e = f/z;

      static std::list<std::string> baseline = {"[", "/", "0.000", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f/Z" ) {
      coek::Expression e = f/Z;

      static std::list<std::string> baseline = {"[", "/", "0.000", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z/f" ) {
      coek::Expression e = z/f;

      static std::list<std::string> baseline = {"[", "/", "z", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/f" ) {
      coek::Expression e = Z/f;

      static std::list<std::string> baseline = {"[", "/", "Z", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = f/a" ) {
      coek::Expression e = f/a;

      static std::list<std::string> baseline = {"[", "/", "0.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/f" ) {
      coek::Expression e = a/f;

      static std::list<std::string> baseline = {"[", "/", "a", "0.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = z/Z" ) {
      coek::Expression e = z/Z;

      static std::list<std::string> baseline = {"[", "/", "z", "Z", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Z/z" ) {
      coek::Expression e = Z/z;

      static std::list<std::string> baseline = {"[", "/", "Z", "z", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test constDivision" ) {

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

  SECTION( "Test trivialDivision" ) {

    coek::Parameter q(0, "q");
    coek::Parameter Q(0.0, "Q");
    coek::Parameter r(1, "r");
    coek::Parameter R(1.0, "R");
    coek::Parameter s(-1, "s");
    coek::Parameter S(-1.0, "S");

    coek::Expression q_(0);
    coek::Expression Q_(0.0);
    coek::Expression r_(1);
    coek::Expression R_(1.0);
    coek::Expression s_(-1);
    coek::Expression S_(-1.0);

    WHEN( "e = q{0} / a" ) {
      coek::Expression e = q/a;

      static std::list<std::string> baseline = {"[","/","q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q_{0} / a" ) {
      coek::Expression e = q_/a;

      static std::list<std::string> baseline = {"[", "/", "0.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 / a" ) {
      coek::Expression e = 0/a;

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} / a" ) {
      coek::Expression e = Q/a;

      static std::list<std::string> baseline = {"[","/","Q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q_{0.0} / a" ) {
      coek::Expression e = Q_/a;

      static std::list<std::string> baseline = {"[", "/", "0.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 / a" ) {
      coek::Expression e = 0.0/a;

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/ r{1}" ) {
      coek::Expression e = a/r;

      static std::list<std::string> baseline = {"[","/","a","r","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/ r_{1}" ) {
      coek::Expression e = a/r_;

      static std::list<std::string> baseline = {"[", "/", "a", "1.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / 1" ) {
      coek::Expression e = a/1;

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / R{1.0}" ) {
      coek::Expression e = a/R;

      static std::list<std::string> baseline = {"[","/","a","R","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / R_{1.0}" ) {
      coek::Expression e = a/R_;

      static std::list<std::string> baseline = {"[", "/", "a", "1.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / 1.0" ) {
      coek::Expression e = a/1.0;

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/ s{-1}" ) {
      coek::Expression e = a/s;

      static std::list<std::string> baseline = {"[","/","a","s","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a/ s_{-1}" ) {
      coek::Expression e = a/s_;

      static std::list<std::string> baseline = {"[", "/", "a", "-1.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / -1" ) {
      coek::Expression e = a/-1;

      static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / S{-1.0}" ) {
      coek::Expression e = a/S;

      static std::list<std::string> baseline = {"[","/","a","S","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / S_{-1.0}" ) {
      coek::Expression e = a/S_;

      static std::list<std::string> baseline = {"[", "/", "a", "-1.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a / -1.0" ) {
      coek::Expression e = a/-1.0;

      static std::list<std::string> baseline = {"[", "*", "-1", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

/*
    WHEN( "e = 3 / q_" ) {
      coek::Expression e = 3/q_;

      REQUIRE( e == 0 );
    }

    WHEN( "e = 3.0 / q_" ) {
      coek::Expression e = 3.0/q_;

      REQUIRE( e == 0 );
    }
*/

  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


/*
TEST_CASE( "capi_pow_expression", "[smoke]" ) {

  apival_t a = create_variable(model, false, false, 0.0, 1.0, 0.0, "a");
  apival_t b = create_variable(model, false, false, 0.0, 1.0, 0.0, "b");

  SECTION( "Test simplePow" ) {
    apival_t e = expr_pow_expression(a,b);

    static std::list<std::string> baseline = {"[", "**", "a", "b", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test constPow" ) {

    WHEN( "e = a**5" ) {
      apival_t e = expr_pow_int(a, 5);

      static std::list<std::string> baseline = {"[", "**", "a", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5**a" ) {
      apival_t e = expr_rpow_int(5, a);

      static std::list<std::string> baseline = {"[", "**", "5.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a**5.0" ) {
      apival_t e = expr_pow_double(a, 5.0);

      static std::list<std::string> baseline = {"[", "**", "a", "5.000", "]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 5.0**a" ) {
      apival_t e = expr_rpow_double(5.0, a);

      static std::list<std::string> baseline = {"[", "**", "5.000", "a", "]"};
      REQUIRE( e.to_list() == baseline );
    }
  }

  SECTION( "Test trivialPow" ) {
    apival_t q = create_parameter_int(model, 0, true, "q");
    apival_t Q = create_parameter_double(model, 0.0, true, "Q");
    apival_t r = create_parameter_int(model, 1, true, "r");
    apival_t R = create_parameter_double(model, 1.0, true, "R");
    apival_t q_ = create_parameter_int(model, 0, false, "");
    apival_t Q_ = create_parameter_double(model, 0.0, false, "");
    apival_t r_ = create_parameter_int(model, 1, false, "");
    apival_t R_ = create_parameter_double(model, 1.0, false, "");

    WHEN( "e = q{0} ** a" ) {
      apival_t e = expr_pow_expression(q, a);

      static std::list<std::string> baseline = {"[","**","q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = q_{0} ** a" ) {
      apival_t e = expr_pow_expression(q_, a);

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0 ** a" ) {
      apival_t e = expr_rpow_int(0, a);

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q{0.0} ** a" ) {
      apival_t e = expr_pow_expression(Q, a);

      static std::list<std::string> baseline = {"[","**","Q","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = Q_{0.0} ** a" ) {
      apival_t e = expr_pow_expression(Q_, a);

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 0.0 ** a" ) {
      apival_t e = expr_rpow_double(0.0, a);

      static std::list<std::string> baseline = {"0.000"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = r{1} ** a" ) {
      apival_t e = expr_pow_expression(r, a);

      static std::list<std::string> baseline = {"[","**","r","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = r_{1} ** a" ) {
      apival_t e = expr_pow_expression(r_, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1 ** a" ) {
      apival_t e = expr_rpow_int(1, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = R{1.0} ** a" ) {
      apival_t e = expr_pow_expression(R, a);

      static std::list<std::string> baseline = {"[","**","R","a","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = R_{1.0} ** a" ) {
      apival_t e = expr_pow_expression(R_, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = 1.0 ** a" ) {
      apival_t e = expr_rpow_double(1.0, a);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** q{0}" ) {
      apival_t e = expr_pow_expression(a, q);

      static std::list<std::string> baseline = {"[","**","a","q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** q_{0}" ) {
      apival_t e = expr_pow_expression(a, q_);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** 0" ) {
      apival_t e = expr_pow_int(a, 0);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** Q{0.0}" ) {
      apival_t e = expr_pow_expression(a, Q);

      static std::list<std::string> baseline = {"[","**","a","Q","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** Q_{0.0}" ) {
      apival_t e = expr_pow_expression(a, Q_);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** 0.0" ) {
      apival_t e = expr_pow_double(a, 0.0);

      static std::list<std::string> baseline = {"1"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** r{1}" ) {
      apival_t e = expr_pow_expression(a, r);

      static std::list<std::string> baseline = {"[","**","a","r","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** r_{1}" ) {
      apival_t e = expr_pow_expression(a, r_);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** 1" ) {
      apival_t e = expr_pow_int(a, 1);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** R{1.0}" ) {
      apival_t e = expr_pow_expression(a, R);

      static std::list<std::string> baseline = {"[","**","a","R","]"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** R_{1.0}" ) {
      apival_t e = expr_pow_expression(a, R_);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }

    WHEN( "e = a ** 1.0" ) {
      apival_t e = expr_pow_double(a, 1.0);

      static std::list<std::string> baseline = {"a"};
      REQUIRE( e.to_list() == baseline );
    }
  }
}
*/


TEST_CASE( "capi_intrinsics", "[smoke]" ) {

{
coek::Variable v("v", 0.0, 1.0, 0.0);
coek::Parameter p(0.0, "p");

  SECTION( "Test abs" ) {
    coek::Expression e = abs(v);
    v.set_value(1);
    REQUIRE( e.get_value() == 1.0 );
    v.set_value(-1);
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"[", "abs", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test ceil" ) {
    coek::Expression e = ceil(v);
    v.set_value(1.5);
    REQUIRE( e.get_value() == 2.0 );
    v.set_value(-1.5);
    REQUIRE( e.get_value() == -1.0 );

    static std::list<std::string> baseline = {"[", "ceil", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test floor" ) {
    coek::Expression e = floor(v);
    v.set_value(1.5);
    REQUIRE( e.get_value() == 1.0 );
    v.set_value(-1.5);
    REQUIRE( e.get_value() == -2.0 );

    static std::list<std::string> baseline = {"[", "floor", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test exp" ) {
    coek::Expression e = exp(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(E) );
    v.set_value(0);
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"[", "exp", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log" ) {
    coek::Expression e = log(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value( exp(1.0) );
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"[", "log", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test log10" ) {
    coek::Expression e = log10(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(10);
    REQUIRE( e.get_value() == 1.0 );

    static std::list<std::string> baseline = {"[", "log10", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test pow" ) {
    coek::Expression e = pow(v,p);
    v.set_value(2);
    p.set_value(0);
    REQUIRE( e.get_value() == Approx(1.0) );
    p.set_value(1);
    REQUIRE( e.get_value() == Approx(2.0) );

    static std::list<std::string> baseline = {"[", "pow", "v", "p", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sqrt" ) {
    coek::Expression e = sqrt(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(1.0) );
    v.set_value(4);
    REQUIRE( e.get_value() == 2.0 );

    static std::list<std::string> baseline = {"[", "sqrt", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sin" ) {
    coek::Expression e = sin(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(PI/2.0);
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "sin", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cos" ) {
    coek::Expression e = cos(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(1.0) );
    v.set_value(PI/2.0);
    REQUIRE( e.get_value() == Approx(0.0).margin(1e-7) );

    static std::list<std::string> baseline = {"[", "cos", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tan" ) {
    coek::Expression e = tan(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(PI/4.0);
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "tan", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asin" ) {
    coek::Expression e = asin(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(PI/2.0) );

    static std::list<std::string> baseline = {"[", "asin", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acos" ) {
    coek::Expression e = acos(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(PI/2.0) );

    static std::list<std::string> baseline = {"[", "acos", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atan" ) {
    coek::Expression e = atan(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(PI/4.0) );

    static std::list<std::string> baseline = {"[", "atan", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test sinh" ) {
    coek::Expression e = sinh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx((E-1/E)/2.0) );

    static std::list<std::string> baseline = {"[", "sinh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test cosh" ) {
    coek::Expression e = cosh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(1.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx((E+1/E)/2.0) );

    static std::list<std::string> baseline = {"[", "cosh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test tanh" ) {
    coek::Expression e = tanh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value(1);
    REQUIRE( e.get_value() == Approx((E-1/E)/(E+1/E)) );

    static std::list<std::string> baseline = {"[", "tanh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test asinh" ) {
    coek::Expression e = asinh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value( (E-1/E)/2.0 );
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "asinh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test acosh" ) {
    coek::Expression e = acosh(v);
    v.set_value(1);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value( (E+1/E)/2.0 );
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "acosh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }

  SECTION( "Test atanh" ) {
    coek::Expression e = atanh(v);
    v.set_value(0);
    REQUIRE( e.get_value() == Approx(0.0) );
    v.set_value( (E-1/E)/(E+1/E) );
    REQUIRE( e.get_value() == Approx(1.0) );

    static std::list<std::string> baseline = {"[", "atanh", "v", "]"};
    REQUIRE( e.to_list() == baseline );
  }
}

#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_constraint", "[smoke]" ) {

{
coek::Variable v("v", 0.0, 1.0, 1.0, false, false);
coek::Parameter p(0.0, "p");
coek::IndexParameter P("P");
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

    // TODO - Error because no variables
    WHEN( "p < p" ) {
        coek::Constraint e = p < p;
        static std::list<std::string> baseline = {"[", "<", "p", "p", "Inf","]"};
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

    // TODO - Error because no variables
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

#if 0
    WHEN( "1 > f" ) {
        coek::Constraint e = 1 > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "-1.000", "0.000", "]", "0", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > 1" ) {
        coek::Constraint e = f > 1;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "-0.000", "1.000", "]", "0","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 > f" ) {
        coek::Constraint e = 1.0 > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "-1.000", "0.000", "]", "0", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > 1.0" ) {
        coek::Constraint e = f > 1.0;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "-0.000", "1.000", "]", "0","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p > p" ) {
        coek::Constraint e = p > p;
        static std::list<std::string> baseline = { "[", "<", "-Inf", "[", "+", "[", "-", "p", "]", "p", "]", "0", "]" };
        REQUIRE( e.to_list() == baseline );
    }
#endif

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

#if 0
    WHEN( "p > f" ) {
        coek::Constraint e = p > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "[", "-", "p", "]", "0.000", "]", "0","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > p" ) {
        coek::Constraint e = f > p;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "-0.000", "p", "]", "0","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P > P" ) {
        coek::Constraint e = P > P;
        static std::list<std::string> baseline = { "[", "<", "-Inf", "[", "+", "[", "-", "P", "]", "P", "]", "0", "]" };
        REQUIRE( e.to_list() == baseline );
    }
#endif

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

#if 0
    WHEN( "P > f" ) {
        coek::Constraint e = P > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "[", "-", "P", "]", "0.000", "]", "0","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f > P" ) {
        coek::Constraint e = f > P;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "-0.000", "P", "]", "0","]"};
        REQUIRE( e.to_list() == baseline );
    }
#endif

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

#if 0
    WHEN( "f > f" ) {
        coek::Constraint e = f > f;
        static std::list<std::string> baseline = {"[", "<", "-Inf", "[", "+", "-0.000", "0.000", "]", "0","]"};
        REQUIRE( e.to_list() == baseline );
    }
#endif
  }

  SECTION( "ge-operator" ) {

#if 0
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
#endif

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

#if 0
    WHEN( "1 >= f" ) {
        coek::Constraint e = 1 >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "-1.000", "0.000", "]", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= 1" ) {
        coek::Constraint e = f >= 1;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "-0.000", "1.000", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "1.0 >= f" ) {
        coek::Constraint e = 1.0 >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "-1.000", "0.000", "]", "0.000", "]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= 1.0" ) {
        coek::Constraint e = f >= 1.0;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "-0.000", "1.000", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "p >= p" ) {
        coek::Constraint e = p >= p;
        static std::list<std::string> baseline = { "[", "<=", "-Inf", "[", "+", "[", "-", "p", "]", "p", "]", "0.000", "]" };
        REQUIRE( e.to_list() == baseline );
    }
#endif

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

#if 0
    WHEN( "p >= f" ) {
        coek::Constraint e = p >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "[", "-", "p", "]", "0.000", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= p" ) {
        coek::Constraint e = f >= p;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "-0.000", "p", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "P >= P" ) {
        coek::Constraint e = P >= P;
        static std::list<std::string> baseline = { "[", "<=", "-Inf", "[", "+", "[", "-", "P", "]", "P", "]", "0.000", "]" };
        REQUIRE( e.to_list() == baseline );
    }
#endif

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

#if 0
    WHEN( "P >= f" ) {
        coek::Constraint e = P >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "[", "-", "P", "]", "0.000", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }

    WHEN( "f >= P" ) {
        coek::Constraint e = f >= P;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "-0.000", "P", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }
#endif

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

#if 0
    WHEN( "f >= f" ) {
        coek::Constraint e = f >= f;
        static std::list<std::string> baseline = {"[", "<=", "-Inf", "[", "+", "-0.000", "0.000", "]", "0.000","]"};
        REQUIRE( e.to_list() == baseline );
    }
#endif
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
        coek::Variable v(0,1,0);
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
    coek::Variable v = m.add_variable("v",0,1,0);
    coek::Constraint a = 0 >= 1 + v;

    repn.collect_terms(a);
    std::stringstream sstr;
    sstr << repn;
    REQUIRE( sstr.str() == "Constant: 1\nLinear: \n1 v\nQuadratic: \n");
  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_compute", "[smoke]" ) {

{
coek::Variable a("a", 0.0, 1.0, 0.0, false, false);
coek::Variable b("b", 0.0, 1.0, 1.0, false, false);
coek::Parameter q(2, "q");

  SECTION( "expression" ) {

    WHEN( "e = q" ) {
        coek::Expression e = q;
        REQUIRE( e.get_value() == 2 );
    }

    WHEN( "e = a" ) {
        coek::Expression e = a;
        REQUIRE( e.get_value() == 0 );
    }

    WHEN( "e = 3*b + q" ) {
        coek::Expression e = 3*b + q;
        REQUIRE( e.get_value() == 5.0 );
    }

  }

  SECTION( "constraint" ) {

    WHEN( "e = 3*b + q == 0" ) {
        coek::Constraint e = 3*b + q == 0;
        REQUIRE( e.body().get_value() == 5.0 );
        REQUIRE( e.is_feasible() == false );
    }
  }

}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}


TEST_CASE( "model_setup", "[smoke]" ) {

{
coek::Model model;
coek::Variable a = model.add_variable("a",0.0, 1.0, 0.0, false, true);
coek::Variable b("b", 0.0, 1.0, 0.0, true, false);
model.add_variable(b);
coek::Parameter q(2, "q");

  SECTION( "add" ) {

    WHEN( "objective" ) {
        coek::Expression e =  3*b + q;
        REQUIRE( model.repn->objectives.size() == 0 );
        model.add_objective( e );
        REQUIRE( model.repn->objectives.size() == 1 );
    }

    WHEN( "inequality" ) {
        coek::Constraint c = 3*b + q <= 0;
        REQUIRE( model.repn->constraints.size() == 0 );
        model.add_constraint(c);
        REQUIRE( model.repn->constraints.size() == 1 );
    }

    WHEN( "equality" ) {
        coek::Constraint c = 3*b + q == 0;
        REQUIRE( model.repn->constraints.size() == 0 );
        model.add_constraint(c);
        REQUIRE( model.repn->constraints.size() == 1 );
    }
  }

  SECTION( "model setup" ) {
    coek::Expression e0 = 3*a + q;
    model.add_objective( e0 );
    coek::Constraint e2 = 3*b + q <= 0;
    model.add_constraint( e2 );
    coek::Constraint e3 = 3*b + q == 0;
    model.add_constraint( e3 );

    WHEN( "print (df == 0)" ) {
        std::stringstream os;
        os << model;
        std::string tmp = os.str();
        REQUIRE( tmp == "MODEL\n\
  Objectives\n\
    min( 3*a + q )\n\
  Constraints\n\
    3*b + q <= 0\n\
    3*b + q == 0\n\
");
    }
  }

/*
        REQUIRE( tmp == "MODEL\n\
  Objectives\n\
    3*b{1} + q{2}\n\
    3*a{0} + q{2}\n\
\n\
  Constraints\n\
    3*b{1} + q{2}  <=  0\n\
    3*b{1} + q{2}  ==  0\n\
" );
*/

#if 0
  SECTION( "model writing" ) {
    coek::Expression e0 = 3*a + q;
    model.add_objective( e0 );
    coek::Constraint e2 = 3*b + q - a <= 0;
    model.add_constraint( e2 );
    coek::Constraint e3 = 3*b + b == 0;
    model.add_constraint( e3 );
    coek::Constraint e4 = 3*b*a + q + b*b + b*b == 0;
    model.add_constraint( e4 );
    coek::Constraint e5 = 3*b*b + q - a*b - a*a <= 0;
    model.add_constraint( e5 );
    coek::Variable c = model.add_variable();
    c.set_lb(0.0);
    coek::Variable d = model.add_variable();
    d.set_ub(0.0);
    model.add (c + d == 0);

    model.write("testing.lp");
  }
#endif


}
#ifdef DEBUG
REQUIRE( coek::env.check_memory() == true );
#endif
}