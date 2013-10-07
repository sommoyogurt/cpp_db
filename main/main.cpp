#include "nullable_types.h"
#include "key_value_pair.h"
#include "coalesce.h"
#include "test.h"

#include <iostream>
#include <memory>
#include <functional>

void test_cpp_db();

int main(int , char *[])
{
    std::cout << std::endl;

    test_stream(&std::cerr);

	test_message("---------- Test testfunctions ----------");
	test_condition("TRUE-Test success", true);
	test_condition("TRUE-Test failed", false);
    test_condition("EXCEPTIONS-Test", []() {if (false) return true; else throw std::runtime_error("This is a test!"); });
    TEST_VERIFY_RESULT(if (false) return true; else throw std::runtime_error("This is a test!"));
	TEST_EQUAL(3, 4711);
	TEST_EQUAL(4, 4);
	TEST_NOT_EQUAL(3, 4711);
	TEST_NOT_EQUAL(4, 4);

	test_message("========== Test starts ==========");

    try
    {
        tools::nullable_int a, c(815);

        a = 4711;

        int b = a;
		tools::nullable_int d;
		const tools::nullable_int e(10), f;

		test_message("---------- Test nullable ----------");

		TEST_NOT_EQUAL(a, 13);
		TEST_EQUAL(a, 4711);
		TEST_EQUAL(b, a);
		TEST_EQUAL(b, 4711);
		TEST_EQUAL(c, 815);

		test_message("---------- Test is_null() ----------");

		test_condition("a is non null: ", !tools::is_null(a) && !a.is_null());
		test_condition("d is null: ", tools::is_null(d) && d.is_null());
		test_condition("e is non null: ", !tools::is_null(e) && !e.is_null());
		test_condition("f is null: ", tools::is_null(f) && f.is_null());
		test_condition("null is null: ", tools::is_null(tools::nullable_int()) && tools::nullable_int().is_null());

		tools::null_type null;

		TEST_VERIFY(tools::is_null(null));
		TEST_VERIFY(!tools::is_null(1));

		test_message("---------- Test coalesce ----------");

		tools::coalesce_trait<int, double, float>::type x0(0);
		tools::coalesce_trait<tools::null_type, double, float>::type x1(0.0);
		tools::coalesce_trait<int, tools::null_type, float>::type x2(0);
		tools::coalesce_trait<tools::null_type, tools::null_type, float>::type x3(0.0f);
		tools::coalesce_trait<int, double, tools::null_type>::type x4(0);
		tools::coalesce_trait<tools::null_type, double, tools::null_type>::type x5(0.0f);
		tools::coalesce_trait<int, tools::null_type, tools::null_type>::type x6(0);
		tools::coalesce_trait<tools::null_type, tools::null_type, tools::null_type>::type x7;

		TEST_EQUAL(tools::coalesce(1, 2, 3, 4), 1);
		TEST_EQUAL(tools::coalesce(tools::null_type(), 2, 3, 4), 2);
		TEST_VERIFY(tools::is_null(tools::coalesce(null, null, null)));
		TEST_EQUAL(tools::coalesce(1, null, null, null), 1);
		TEST_EQUAL(tools::coalesce(null, 2, null, 4), 2);
            
		TEST_EQUAL(x0, 0);
		TEST_EQUAL(x1, 0.0);
		TEST_EQUAL(x2, 0);
		TEST_EQUAL(x3, 0.0f);
		TEST_EQUAL(x4, 0);
		TEST_EQUAL(x5, 0.0f);
		TEST_EQUAL(x6, 0);
		TEST_VERIFY(is_null(x7));

		std::unique_ptr<tools::connection_option> p(tools::make_key_value_pair(std::string("driver"), std::string("QIBASE")));
		TEST_EQUAL( (p->key<std::string, std::string>()), "driver");
		TEST_EQUAL( (p->value<std::string, std::string>()) , std::string("QIBASE"));

		test_message("========== Test ends ==========");

		test_cpp_db();
	}
    catch(const std::exception &ex)
    {
		test_message(std::string("Exception: ") + ex.what());
	}
}