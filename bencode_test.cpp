#include "encode.h"
#include "value.h"
#include "decode.h"

#include "utestpp/utestpp.h"

#include <iterator>
#include <iostream>
#include <stdexcept>
#include <string>

using bencode::encode;
using bencode::decode;
using bencode::value;

void test_int()
{
	ASSERT_EQUAL(decode("i3e"), value(3));
	ASSERT_EQUAL(decode("i0e"), value(0));
	ASSERT_EQUAL(decode("i1099511627776e"), value(1099511627776ll));
	ASSERT_EQUAL(decode("i-3e"), value(-3));
	ASSERT_EQUAL(encode(3), "i3e");
	ASSERT_EQUAL(encode(0), "i0e");
	ASSERT_EQUAL(encode(1099511627776ll), "i1099511627776e");
	ASSERT_EQUAL(encode(-17), "i-17e");
	
	ASSERT_THROWS(decode("ie"));
	ASSERT_THROWS(decode("i123ae"));
	ASSERT_THROWS(decode("i123 e"));
	ASSERT_THROWS(decode("i 123e"));
	ASSERT_THROWS(decode("i 123 e"));
	ASSERT_THROWS(decode("i e"));
	ASSERT_THROWS(decode("iabce"));
	
	//Technically invalid by bencode standard, but not worth rejection over
	//ASSERT_THROWS(decode("i-0e"));
}

void test_string()
{
	ASSERT_EQUAL(decode("5:hello"), value("hello"));
	ASSERT_EQUAL(decode("11:hello world"), value("hello world"));
	ASSERT_EQUAL(decode("9:!!!333@@@"), value("!!!333@@@"));
	ASSERT_EQUAL(decode("0:"), value(""));

	ASSERT_THROWS(decode("9:!!!"));
	ASSERT_THROWS(decode("-3:!!!"));
	ASSERT_THROWS(decode("abc:!!!"));
	ASSERT_THROWS(decode(":"));
	ASSERT_THROWS(decode(":abc"));
	ASSERT_THROWS(decode("3:"));
	ASSERT_THROWS(decode("3"));
	ASSERT_THROWS(decode("a:1"));
	ASSERT_THROWS(decode(""));
}

void test_list()
{
	value::list_type simple_list_vector{value("spam"), value("eggs")};
	value simple_list(simple_list_vector);
	std::string simple_list_string("l4:spam4:eggse");
	
	value::list_type nested_list_vector{value(35), simple_list};
	value nested_list(nested_list_vector);
	std::string nested_list_string("li35e" + simple_list_string + "e");
	
	ASSERT_EQUAL(decode(simple_list_string), simple_list);
	ASSERT_EQUAL(decode(nested_list_string), nested_list);
}

void test_dict()
{
	value::dict_type simple_dict_map;
	simple_dict_map[value("foo")] = decode("l3:bar3:baze");
	value simple_dict(simple_dict_map);
	std::string simple_dict_string("d3:fool3:bar3:bazee");
	
	value::list_type simple_dict_list_vector{value("foo"), simple_dict};
	value simple_dict_list(simple_dict_list_vector);
	std::string simple_dict_list_string("l3:foo" + simple_dict_string + "e");
	
	ASSERT_EQUAL(decode(simple_dict_string), simple_dict);
	ASSERT_EQUAL(decode(simple_dict_list_string), simple_dict_list);
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
