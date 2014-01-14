#include "bencode.h"
#include "bencode_value.h"
#include "bdecode.h"

#include "utestpp/utestpp.h"

#include <fstream>
#include <iterator>
#include <iostream>
#include <stdexcept>
#include <string>

void test_int()
{
	ASSERT_EQUAL(bdecode("i3e"), bencode_value(3));
	ASSERT_EQUAL(bdecode("i0e"), bencode_value(0));
	ASSERT_EQUAL(bdecode("i1099511627776e"), bencode_value(1099511627776ll));
	ASSERT_EQUAL(bdecode("i-3e"), bencode_value(-3));
	ASSERT_EQUAL(bencode(3), "i3e");
	ASSERT_EQUAL(bencode(0), "i0e");
	ASSERT_EQUAL(bencode(1099511627776ll), "i1099511627776e");
	ASSERT_EQUAL(bencode(-17), "i-17e");
	
	ASSERT_THROWS(bdecode("ie"));
	ASSERT_THROWS(bdecode("i123ae"));
	ASSERT_THROWS(bdecode("i123 e"));
	ASSERT_THROWS(bdecode("i 123e"));
	ASSERT_THROWS(bdecode("i 123 e"));
	ASSERT_THROWS(bdecode("i e"));
	ASSERT_THROWS(bdecode("iabce"));
	
	//Technically invalid by bencode standard, but not worth rejection over
	//ASSERT_THROWS(bdecode("i-0e"));
}

void test_string()
{
	ASSERT_EQUAL(bdecode("5:hello"), bencode_value("hello"));
	ASSERT_EQUAL(bdecode("11:hello world"), bencode_value("hello world"));
	ASSERT_EQUAL(bdecode("9:!!!333@@@"), bencode_value("!!!333@@@"));
	ASSERT_EQUAL(bdecode("0:"), bencode_value(""));

	ASSERT_THROWS(bdecode("9:!!!"));
	ASSERT_THROWS(bdecode("-3:!!!"));
	ASSERT_THROWS(bdecode("abc:!!!"));
	ASSERT_THROWS(bdecode(":"));
	ASSERT_THROWS(bdecode(":abc"));
	ASSERT_THROWS(bdecode("3:"));
	ASSERT_THROWS(bdecode("3"));
	ASSERT_THROWS(bdecode("a:1"));
	ASSERT_THROWS(bdecode(""));
}

void test_list()
{
	bencode_value::list_type simple_list_vector{bencode_value("spam"), bencode_value("eggs")};
	bencode_value simple_list(simple_list_vector);
	std::string simple_list_string("l4:spam4:eggse");
	
	bencode_value::list_type nested_list_vector{bencode_value(35), simple_list};
	bencode_value nested_list(nested_list_vector);
	std::string nested_list_string("li35e" + simple_list_string + "e");
	
	ASSERT_EQUAL(bdecode(simple_list_string), simple_list);
	ASSERT_EQUAL(bdecode(nested_list_string), nested_list);
}

void test_dict()
{
	bencode_value::dict_type simple_dict_map;
	simple_dict_map[bencode_value("foo")] = bdecode("l3:bar3:baze");
	bencode_value simple_dict(simple_dict_map);
	std::string simple_dict_string("d3:fool3:bar3:bazee");
	
	bencode_value::list_type simple_dict_list_vector{bencode_value("foo"), simple_dict};
	bencode_value simple_dict_list(simple_dict_list_vector);
	std::string simple_dict_list_string("l3:foo" + simple_dict_string + "e");
	
	ASSERT_EQUAL(bdecode(simple_dict_string), simple_dict);
	ASSERT_EQUAL(bdecode(simple_dict_list_string), simple_dict_list);
}

int main()
{
	UTESTPP_INIT();
	test_int();
	test_string();
	test_list();
	test_dict();
	UTESTPP_FINISH();
}
