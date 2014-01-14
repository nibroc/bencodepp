#include "value.h"
#include "encode.h"

using namespace bencode;

std::string bencode::encode(const std::string& str)
{
	return std::to_string(str.size()) + ":" + str;
}

std::string bencode::encode(std::int64_t i)
{
	return "i" + std::to_string(i) + "e";
}

std::string bencode::encode(const value& val)
{
	return val.encode();
}

std::string bencode::encode(const value::list_type& list)
{
	std::string lstr;
	for (const value& val : list) {
		lstr += bencode::encode(val);
	}
	return "l" + lstr + "e";
}

std::string bencode::encode(const value::dict_type& dict)
{
	std::string dstr;
	for (const value::dict_type::value_type& v : dict) {
		dstr += encode(v.first) + encode(v.second);
	}
	return "d" + dstr + "e";
}
