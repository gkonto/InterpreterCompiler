#include "VM.hpp"
#include "ASTBuilder.hpp"
#include "ASTNode.hpp"
#include "SyntaxParser.hpp"
#include "TByteCodeBuilder.hpp"
#include "TModule.hpp"
#include "ast.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

static void checkSyntaxParserErrors(std::optional<SyntaxError> error)
{
    INFO("SyntaxError found: " << (error.has_value() ? error.value().msg()
                                                     : ""));
    REQUIRE(!error.has_value());
}

template <typename T>
static void testVM(const std::string &input,
                   TStackRecordType expected_type,
                   T expected_value)
{
    std::istringstream iss(input);
    Scanner sc(iss);
    SyntaxParser sp(sc);
    auto err = sp.syntaxCheck();
    checkSyntaxParserErrors(err);
    TByteCodeBuilder builder(sp.tokens());
    auto module = std::make_shared<TModule>();
    constantValueTable.clear();
    builder.build(module.get());

    VM vm;
    vm.runModule(module);
    REQUIRE(vm.empty() == false);
    const auto &result = vm.top();
    INFO(
        "Input> \n'" + input + "'\nExpected: Type>" +
        TStackRecordTypeToStr(expected_type) + " Value> " +
        std::to_string(expected_value)
        // + "\nGot   : Type>" +  TStackRecordTypeToStr(result.type()) + " Value> " + result.value()
    );
    REQUIRE(result.type() == expected_type);

    if (result.type() == TStackRecordType::stInteger)
    {
        REQUIRE(result.ivalue() == expected_value);
    }
    else if (result.type() == TStackRecordType::stDouble)
    {
        REQUIRE(std::abs(result.dvalue() - expected_value) < 1e-6);
    }
    else if (result.type() == TStackRecordType::stBoolean)
    {
        REQUIRE(result.bvalue() == expected_value);
    }
    else
    {
        REQUIRE(false);
    }
}

static std::string fn_call_fib25()
{
    return "fn fibonacci(n)\n"
           "    if n < 2 then\n"
           "        return n\n"
           "    end\n"
           "    return fibonacci(n - 1) + fibonacci(n - 2)\n"
           "end;\n"
           "\n"
           "fibonacci(25);\n";
}

static std::string fn_call_fib28()
{
    return "fn fibonacci(n)\n"
           "    if n < 2 then\n"
           "        return n\n"
           "    end\n"
           "    return fibonacci(n - 1) + fibonacci(n - 2)\n"
           "end;\n"
           "\n"
           "fibonacci(28);\n";
}

static std::string fn_call_fib30()
{
    return "fn fibonacci(n)\n"
           "    if n < 2 then\n"
           "        return n\n"
           "    end\n"
           "    return fibonacci(n - 1) + fibonacci(n - 2)\n"
           "end;\n"
           "\n"
           "fibonacci(28);\n";
}

static std::string fn_call_fib31()
{
    return "fn fibonacci(n)\n"
           "    if n < 2 then\n"
           "        return n\n"
           "    end\n"
           "    return fibonacci(n - 1) + fibonacci(n - 2)\n"
           "end;\n"
           "\n"
           "fibonacci(31);\n";
}

static std::string fn_call_fib33()
{
    return "fn fibonacci(n)\n"
           "    if n < 2 then\n"
           "        return n\n"
           "    end\n"
           "    return fibonacci(n - 1) + fibonacci(n - 2)\n"
           "end;\n"
           "\n"
           "fibonacci(33);\n";
}

static std::string fn_call_fib35()
{
    return "fn fibonacci(n)\n"
           "    if n < 2 then\n"
           "        return n\n"
           "    end\n"
           "    return fibonacci(n - 1) + fibonacci(n - 2)\n"
           "end;\n"
           "\n"
           "fibonacci(35);\n";
}

static std::string input_conditionals_1()
{
    return "let a = false;\n"
           "if true then\n"
           "   a = true;\n"
           "end\n"
           "a == true;";
}

static std::string input_conditionals_2()
{
    return "let a = true;\n"
           "if true then\n"
           "    a = false\n"
           "end;\n"
           "a == false;";
}

static std::string input_conditionals_3()
{
    return "let a = true;\n"
           "if false then\n"
           "    a = false;\n"
           "else\n"
           "    a = true;\n"
           "end;\n"
           "a";
}

static std::string input_conditionals_4()
{
    return "let x = 5;\n"
           "let y = 10;\n"
           "let a = false;\n"
           "if x > y then\n"
           "    a = false;\n"
           "else\n"
           "    a = true;\n"
           "end;\n"
           "a";
}

static std::string input_conditionals_5()
{
    return "let x = 5;\n"
           "let y = 10;\n"
           "let a = false;\n"
           "if x < y then\n"
           "    a = true;\n"
           "else\n"
           "    a = false;\n"
           "end;\n"
           "a";
}

static std::string input_conditionals_6()
{
    return "let x = 10; \n"
           "let y = 5;  \n"
           "let a = false;\n"
           "\n"
           "if x > y then \n"
           "   a = true\n"
           "else   \n"
           "   a = false\n"
           "end;\n"
           "a;\n"
           "\n";
}

static std::string input_conditionals_7()
{
    return "let x = 10; \n"
           "let y = 5;  \n"
           "let a = false;\n"
           "if x < y then\n"
           "   a = false\n"
           "else    \n"
           "   a = true\n"
           "end;\n"
           "a;";
}

static std::string input_conditionals_8()
{
    return "let a = 10;\n"
           "if true then\n"
           "  a = 1;\n"
           "  if true then\n"
           "     a = 2;\n"
           "     if false then\n"
           "        a = 3;\n"
           "     end;\n"
           "  end;\n"
           "end;\n"
           "a;";
}

static std::string input_conditionals_9()
{
    return "let a = 10;\n"
           "if true then\n"
           "  a = 1;\n"
           "  if true then\n"
           "     a = 2;\n"
           "     if true then\n"
           "        a = 3;\n"
           "     end;\n"
           "  end;\n"
           "end;\n"
           "a;\n";
}

static std::string input_conditionals_10()
{
    return "let a= 10;\n"
           "if false then\n"
           "   a = 1;\n"
           "else\n"
           "  if true then\n"
           "     a = 2\n"
           "  end;\n"
           "end;\n"
           "a;    \n";
}

static std::string input_conditionals_11()
{
    return "let a = 10;\n"
           "if false then\n"
           "   a = 1;\n"
           "else\n"
           "  if false then\n"
           "     a = 2\n"
           "  else\n"
           "     a = 3;\n"
           "   end;\n"
           "end;\n"
           "a;    \n";
}

static std::string input_conditionals_12()
{
    return "let a = 11;\n"
           "if true then\n"
           "   a = 1;\n"
           "else\n"
           "  if false then\n"
           "     a = 2\n"
           "  else\n"
           "     if false then\n"
           "        a = 3;\n"
           "     else\n"
           "        a = 4;\n"
           "     end;\n"
           "  end;\n"
           "end;\n"
           "a;\n";
}

static std::string input_conditionals_13()
{
    return "let a= 44;\n"
           "if false then\n"
           "   a = 1;\n"
           "else\n"
           "  if true then\n"
           "     a = 2\n"
           "  else\n"
           "     if false then\n"
           "        a = 3;\n"
           "     else\n"
           "        a = 4;\n"
           "     end;\n"
           "  end;\n"
           "end;\n"
           "a;\n";
}

static std::string input_conditionals_14()
{
    return "let a = 10;\n"
           "if false then\n"
           "   a = 1;\n"
           "else\n"
           "  if false then\n"
           "     a = 2\n"
           "  else\n"
           "     if true then\n"
           "        a = 3;\n"
           "     else\n"
           "        a = 4;\n"
           "     end;\n"
           "  end;\n"
           "end;\n"
           "a;    \n";
}

static std::string input_conditionals_15()
{
    return "let a = 645\n"
           "if false then\n"
           "   a = 1;\n"
           "else\n"
           "  if false then\n"
           "     a = 2\n"
           "  else\n"
           "     if false then\n"
           "        a = 3;\n"
           "     else\n"
           "        a = 4;\n"
           "     end;\n"
           "  end;\n"
           "end;\n"
           "a;\n";
}

static std::string fn_call_i1()
{
    return "fn rint()\n"
           "  return 777;\n"
           "end;\n"
           "rint();\n";
}

static std::string fn_call_i2()
{
    return "fn rint()\n"
           "    let x = 15;\n"
           "    return x;\n"
           "end;\n"
           "rint();";
}

static std::string fn_call_i3()
{
    return "fn test_1()\n"
           "   let x = 1234;\n"
           "   x = 9;\n"
           "   return x\n"
           "end;\n"
           "test_1();\n";
}

static std::string fn_call_i4()
{
    return "fn test_i3()\n"
           "    let x = 15;\n"
           "   return x + 10;\n"
           "end;\n"
           "test_i3();\n";
}

static std::string fn_call_i5()
{
    return "fn test_1()\n"
           "   let x = 10;\n"
           "    let y = 25;\n"
           "   return x + y\n"
           "end;\n"
           "test_1();\n";
}

static std::string fn_call_b1()
{
    return "fn rbool()\n"
           "  return true;\n"
           "end;\n"
           "rbool();\n";
}

static std::string fn_call_b2()
{
    return "fn rbool()\n"
           "    let x = true;"
           "  return x;\n"
           "end;\n"
           "rbool();\n";
}

static std::string fn_call_d1()
{
    return "fn rdouble()\n"
           "    return 3.1415;\n"
           "end;\n"
           "rdouble();\n";
}

static std::string fn_call_d2()
{
    return "fn rdouble()\n"
           "   let x = 2.1;"
           "    return x;\n"
           "end;\n"
           "rdouble();\n";
}

static std::string fn_call_d3()
{
    return "fn test_4()\n"
           "   let c = 1.1;\n"
           "   if true then\n"
           "     let x = c + 0.1;\n"
           "     return x\n"
           "   end;\n"
           "end;\n"
           "test_4();\n";
}

static std::string fn_call_i6()
{
    return "fn test_6()\n"
           "    let x = 6; \n"
           "    if x == 5 then\n"
           "       x = 0\n"
           "    else\n"
           "       return 123456;\n"
           "    end;\n"
           " end;\n"
           " test_6();   \n";
}

static std::string fn_call_i7()
{
    return "fn mean(x)\n"
           "    return x;\n"
           "end;\n"
           "let x = mean(12);\n"
           "x\n";
}

static std::string fn_call_i8()
{
    return "fn mean(x)\n"
           "    let a = x;"
           "    return a;\n"
           "end;\n"
           "let x = mean(13);\n"
           "x\n";
}

static std::string fn_call_i9()
{
    return "let y = 99;\n"
           "fn mean(x)\n"
           "    let a = x;"
           "    return a;\n"
           "end;\n"
           "let x = y + mean(13);\n"
           "x\n";
}

static std::string fn_call_d4()
{
    return "let x = 1.45678;\n"
           "fn testGlobal_1 ()\n"
           "   let x = 4.5;   \n"
           "end;\n"
           "testGlobal_1();"
           "x\n";
}

static std::string fn_call_i10()
{
    return "fn fac(x)\n"
           "    if x <= 0 then\n"
           "        return 1;\n"
           "    end;\n"
           "    return (x * fac(x-1));\n"
           "end;\n"
           "fac(0);";
}

static std::string fn_call_i11()
{
    return "fn fac(x)\n"
           "    if x <= 0 then\n"
           "        return 1;\n"
           "    end;\n"
           "    return (x * fac(x-1));\n"
           "end;\n"
           "fac(1);";
}

static std::string fn_call_i12()
{
    return "fn fac(x)\n"
           "    if x <= 0 then\n"
           "        return 1;\n"
           "    end;\n"
           "    return (x * fac(x-1));\n"
           "end;\n"
           "fac(2);";
}

static std::string fn_call_i13()
{
    return "fn fac(x)\n"
           "    if x <= 0 then\n"
           "        return 1;\n"
           "    end;\n"
           "    return (x * fac(x-1));\n"
           "end;\n"
           "fac(5);";
}

static std::string fn_call_i14()
{
    return "fn fac(x)\n"
           "    if x <= 0 then\n"
           "        return 1;\n"
           "    end;\n"
           "    return (x * fac(x-1));\n"
           "end;\n"
           "fac(10);";
}

static std::string fn_call_i15()
{
    return "fn add(a, b)\n"
           "   return a + b;\n"
           "end;\n"
           "add(15, 10);";
}

static std::string fn_call_i16()
{
    return "fn add (a, b)\n"
           "    return a + b\n"
           "end;\n"
           "\n"
           "fn sumOfAdds (a, b)\n"
           "    return add (a, b) + add (a, b)\n"
           "end;\n"
           "sumOfAdds(2, 4)";
}

static std::string fn_call_i17()
{
    return "fn fibonacci(n)\n"
           "    if n < 2 then\n"
           "        return n\n"
           "    end\n"
           "    return fibonacci(n - 1) + fibonacci(n - 2)\n"
           "end;\n"
           "\n"
           "fibonacci(10);\n";
}

static std::string fn_call_i18()
{
    return "fn fibonacci(n)\n"
           "    if n < 2 then\n"
           "        return n\n"
           "    end\n"
           "    return fibonacci(n - 1) + fibonacci(n - 2)\n"
           "end;\n"
           "\n"
           "fibonacci(26);\n";
}

TEST_CASE("Test_VM_arithmetic", "[quick]")
{
    SECTION("Simple")
    {
        std::vector<std::tuple<std::string, TStackRecordType, double>> tests = {
            {"1E2", TStackRecordType::stDouble, 100},
            {"1.2E2", TStackRecordType::stDouble, 120},
            {"0.1E2 ", TStackRecordType::stDouble, 10},
            {"1E-2 ", TStackRecordType::stDouble, 0.01},
            {"1.2E-2 ", TStackRecordType::stDouble, 0.012},
            {"-1E-2 ", TStackRecordType::stDouble, -0.01},
            {"2 + 3 ", TStackRecordType::stInteger, 5},
            {"2 + 3.0 ", TStackRecordType::stDouble, 5},
            {"2 - 3 ", TStackRecordType::stInteger, -1},
            {"3 - 2 ", TStackRecordType::stInteger, 1},
            {"3 - 2.0 ", TStackRecordType::stDouble, 1},
            {"2 * 3 ", TStackRecordType::stInteger, 6},
            {"2 * 3.0 ", TStackRecordType::stDouble, 6},
            {"10 / 2 ", TStackRecordType::stInteger, 5},
            {"10 / 2.0", TStackRecordType::stDouble, 5},
            {"2 ^ 4 ", TStackRecordType::stDouble, 16},
            {"let a = 123.456; a", TStackRecordType::stDouble, 123.456},
            {"let a = 0.00001; a", TStackRecordType::stDouble, 0.00001},
            {"let a = -0.00001; a", TStackRecordType::stDouble, -0.00001},
            {"let a = 123.; a", TStackRecordType::stDouble, 123},
            {"let a = .987; a", TStackRecordType::stDouble, 0.987},
            {"let x = 1234; x = 9; x", TStackRecordType::stInteger, 9}};

        for (const auto &[input, expected_type, expected_value] : tests)
        {
            testVM(input, expected_type, expected_value);
        }
    }

    SECTION("Boolean")
    {
        std::vector<std::tuple<std::string, TStackRecordType, bool>> tests = {
            {"let a = 2; a == 2", TStackRecordType::stBoolean, true},
            {"let a = 2; let b = 5;b == 3", TStackRecordType::stBoolean, false},
            {"false == (2 < 1)", TStackRecordType::stBoolean, true},
        };
        for (const auto &[input, expected_type, expected_value] : tests)
        {
            testVM(input, expected_type, expected_value);
        }
    }

    SECTION("Integer")
    {
        std::vector<std::tuple<std::string, TStackRecordType, int>> tests = {
            {"let a = 2; let b = 5; a", TStackRecordType::stInteger, 2},
            {"let a = 2; let b = 5; b", TStackRecordType::stInteger, 5},
            {"let a = 2; let b = 5; a-b", TStackRecordType::stInteger, -3},
            {"let a = 2; let b = 5; b-a", TStackRecordType::stInteger, 3},
            {"let a = 2; let b = 5; a * b", TStackRecordType::stInteger, 10},
            {"let a = 2; let b = 5; b * a", TStackRecordType::stInteger, 10},
            {"let a = 2; let b = 5; b / a", TStackRecordType::stInteger, 2},
            {"let a = 2; let b = 5; a / b", TStackRecordType::stInteger, 0},
        };

        for (const auto &[input, expected_type, expected_value] : tests)
        {
            testVM(input, expected_type, expected_value);
        }
    }

    SECTION("Float")
    {
        std::vector<std::tuple<std::string, TStackRecordType, double>> tests = {
            {"let a = 2.0; let b = 5.0; a", TStackRecordType::stDouble, 2},
            {"let a = 2; let b = 5.0; b", TStackRecordType::stDouble, 5},
            {"let a = 2; let b = 5.0; a-b", TStackRecordType::stDouble, -3},
            {"let a = 2; let b = 5.0; b-a", TStackRecordType::stDouble, 3},
            {"let a = 2; let b = 5.0; a * b", TStackRecordType::stDouble, 10},
            {"let a = 2; let b = 5.0; b * a", TStackRecordType::stDouble, 10},
            {"let a = 2; let b = 5.0; b / a", TStackRecordType::stDouble, 2.5},
            {"let a = 2; let b = 5.0; a / b", TStackRecordType::stDouble, 0.4},
            {"let a = 2.5; let b = 5.5; a+b", TStackRecordType::stDouble, 8.0},
            {"let a = 2.5; let b = 5.5; b+a", TStackRecordType::stDouble, 8.0},
            {"let a = 2.5; let b = 5.5; a-b", TStackRecordType::stDouble, -3.0},
            {"let a = 2.5; let b = 5.5; b-a", TStackRecordType::stDouble, 3.0},
            {"let a = 2.5; let b = 5.5; a*b",
             TStackRecordType::stDouble,
             13.75},
            {"let a = 2.5; let b = 5.5; b*a",
             TStackRecordType::stDouble,
             13.75},
            {"let a = 2.5; let b = 5.5; b/a", TStackRecordType::stDouble, 2.2},
            {"let a = 2.5; let b = 5.5; b = 7.5; a/b",
             TStackRecordType::stDouble,
             1 / 3.0},
            {"let a = 2.5; let b = 5.5; b = 2.0; a = 1.5; a^b",
             TStackRecordType::stDouble,
             2.25},
            {"8/2.5", TStackRecordType::stDouble, 3.2},
            {"2.5/8", TStackRecordType::stDouble, 0.3125},
        };

        for (const auto &[input, expected_type, expected_value] : tests)
        {
            testVM(input, expected_type, expected_value);
        }
    }

    SECTION("Precedence")
    {
        std::vector<std::tuple<std::string, TStackRecordType, int>> tests = {
            {"5 * 4 + 2", TStackRecordType::stInteger, 22},
            {"2 + 5 * 4", TStackRecordType::stInteger, 22},
            {"(2 + 5) * 4", TStackRecordType::stInteger, 28},
            {"2 + 6 / 3", TStackRecordType::stInteger, 4},
            {"2 - 6 / 3", TStackRecordType::stInteger, 0},
        };

        for (const auto &[input, expected_type, expected_value] : tests)
        {
            testVM(input, expected_type, expected_value);
        }
    }
}

TEST_CASE("Test_VM_Booleans", "[quick]")
{
    SECTION("Precedence")
    {
        std::vector<std::tuple<std::string, bool>> tests = {
            {"true == true", true},
            {"true != false", true},
            {"true != true", false},
            {"false != false", false},

            {"not false", true},
            {"not true", false},
            {"not not true", true},

            {"true or true", true},
            {"true or false", true},
            {"false or true", true},
            {"false and true", false},
            {"true and false", false},
            {"false and false", false},

            {"not (false or true);", false},
            {"not (false or false);", true},
            {"not (false and true);", true},
            {"not (true and false);", true},
            {"not (false and false);", true},

            {"let x = 1;x < x and x > x;", false},
            {"((1 >= 6) and not (3 <= 4));", false},
            {"let y1 = 6; y1 != 6;", false},
            {"let y1 = 6;y1 == 6;", true},

            {"1 < 2;", true},
            {"1 <= 2;", true},
            {"not (1 > 2);", true},
            {"not (1 >= 2);", true},
            {"not (1 == 2);", true},
            {"(1 != 2);", true},

            {"1 < 1 and 1 > 1", false},
            {"(1 >=6) and not (3 <=4)", false},
            {"let y = 6; (y == 6) == true", true},
        };

        for (const auto &[input, expected_value] : tests)
        {
            testVM(input, TStackRecordType::stBoolean, expected_value);
        }
    }
}

TEST_CASE("Test_VM_Conditionals", "[quick]")
{
    SECTION("Boolean")
    {
        std::vector<std::tuple<std::string, bool>> tests = {
            {input_conditionals_1(), true},
            {input_conditionals_2(), true},
            {input_conditionals_3(), true},
            {input_conditionals_4(), true},
            {input_conditionals_5(), true},
            {input_conditionals_6(), true},
            {input_conditionals_7(), true},
        };
        for (const auto &[input, expected_value] : tests)
        {
            testVM(input, TStackRecordType::stBoolean, expected_value);
        }
    }

    SECTION("Integers")
    {
        std::vector<std::tuple<std::string, int>> tests = {
            {input_conditionals_8(), 2},
            {input_conditionals_9(), 3},
            {input_conditionals_10(), 2},
            {input_conditionals_11(), 3},
            {input_conditionals_12(), 1},
            {input_conditionals_13(), 2},
            {input_conditionals_14(), 3},
            {input_conditionals_15(), 4},
        };
        for (const auto &[input, expected_value] : tests)
        {
            testVM(input, TStackRecordType::stInteger, expected_value);
        }
    }
}

TEST_CASE("Test_VM_FunctionCalls", "[quick]")
{
    SECTION("Integers")
    {
        std::vector<std::tuple<std::string, int>> tests = {
            {fn_call_i1(), 777},
            {fn_call_i2(), 15},
            {fn_call_i3(), 9},
            {fn_call_i4(), 25},
            {fn_call_i5(), 35},
            {fn_call_i6(), 123456},
            {fn_call_i7(), 12},
            {fn_call_i8(), 13},
            {fn_call_i9(), 112},
            {fn_call_i10(), 1},
            {fn_call_i11(), 1},
            {fn_call_i12(), 2},
            {fn_call_i13(), 120},
            {fn_call_i14(), 3628800},
            {fn_call_i15(), 25},
            {fn_call_i16(), 12},
            {fn_call_i17(), 55},
            {fn_call_i18(), 121393}};
        for (const auto &[input, expected_value] : tests)
        {
            testVM(input, TStackRecordType::stInteger, expected_value);
        }
    }

    SECTION("Booleans")
    {
        std::vector<std::tuple<std::string, bool>> tests = {
            {fn_call_b1(), true},
            {fn_call_b2(), true},
        };
        for (const auto &[input, expected_value] : tests)
        {
            testVM(input, TStackRecordType::stBoolean, expected_value);
        }
    }

    SECTION("Double")
    {
        std::vector<std::tuple<std::string, double>> tests = {
            {fn_call_d1(), 3.1415},
            {fn_call_d2(), 2.1},
            {fn_call_d3(), 1.2},
            {fn_call_d4(), 1.45678}};
        for (const auto &[input, expected_value] : tests)
        {
            testVM(input, TStackRecordType::stDouble, expected_value);
        }
    }
}