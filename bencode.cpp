#include "bencode_value.h"

#include "bencode.h"

std::string bencode(const std::string& str)
{
	return std::to_string(str.size()) + ":" + str;
}

std::string bencode(std::int64_t i)
{
	return "i" + std::to_string(i) + "e";
}

std::string bencode(const bencode_value& val)
{
	return val.bencode();
}

std::string bencode(const bencode_value::list_type& list)
{
	std::string lstr;
	for (const bencode_value& val : list) {
		lstr += bencode(val);
	}
	return "l" + lstr + "e";
}

std::string bencode(const bencode_value::dict_type& dict)
{
	std::string dstr;
	for (const bencode_value::dict_type::value_type& v : dict) {
		dstr += bencode(v.first) + bencode(v.second);
	}
	return "d" + dstr + "e";
}
