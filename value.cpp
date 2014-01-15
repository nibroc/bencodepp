#include "value.h"

#include <stdexcept>

static std::string list_to_string(const bencode::value::list_type& l);

static std::string dict_to_string(const bencode::value::dict_type& d);

namespace bencode {

value::value()
  : type_(type::null)
{ }

value::value(const int_type v) 
  : type_(type::integer)
{
	construct(v);
}

value::value(const string_type& str) 
  : type_(type::string)
{
	construct(str);
}

value::value(const list_type& list) 
  : type_(type::list)
{
	construct(list);
}

value::value(const dict_type& dict)
  : type_(type::dict)
{
	construct(dict);
}

value::value(const value& v) : type_(v.type_)
{
	switch(v.type_) {
		case type::integer:
			copy_type(v.int_value());
			break;
		case type::string:
			copy_type(v.string_value());
			break;
		case type::list:
			copy_type(v.list_value());
			break;
		case type::dict:
			copy_type(v.dict_value());
			break;
		default:
			break;
	}
}

value::~value()
{
	switch (type_) {
		case type::integer:
			destruct_type<int_type>();
			break;
		case type::string:
			destruct_type<string_type>();
			break;
		case type::list:
			destruct_type<list_type>();
			break;
		case type::dict:
			destruct_type<dict_type>();
			break;
		case type::null:
			/* No need to do anything when empty */
			break;
	}
}

std::string value::encode() const
{
	switch (type_) {
		case type::integer:
			return bencode::encode(int_value());
		case type::string:
			return bencode::encode(string_value());
		case type::list:
			return bencode::encode(list_value());
		case type::dict:
			return bencode::encode(dict_value());
		case type::null:
			throw std::runtime_error("Cannot dereference null bencode values");
		default:
			throw std::runtime_error("Unknown value type");
	}
}

bool bencode::value::operator<(const value& other) const
{
	return encode() < other.encode();
}

bool bencode::value::operator==(const value& other)
{
	if (type_ != other.type_) {
		return false;
	}
	return encode() == other.encode();
}

std::string bencode::value::to_string() const
{
	switch (type_) {
		case type::integer:
			return std::to_string(int_value());
		case type::string:
			return string_value();
		case type::list:
			return list_to_string(list_value());
		case type::dict:
			return dict_to_string(dict_value());
		case type::null:
			throw std::runtime_error("Cannot dereference null bencode values");
		default:
			throw std::runtime_error("Unknown value type");
	}
}

}

static std::string list_to_string(const bencode::value::list_type& l)
{
	std::string s;
	bool suppress_comma = true;
	for (const auto& v : l) {
		if (suppress_comma) {
			suppress_comma = false;
		} else {
			s += ", ";
		}
		s += v.to_string();
	}
	return "[" + s + "]";
}

static std::string dict_to_string(const bencode::value::dict_type& d)
{
	std::string s;
	bool suppress_comma = true;
	for (const auto& v : d) {
		if (suppress_comma) {
			suppress_comma = false;
		} else {
			s += ", ";
		}
		s += v.first.to_string() + ": " + v.second.to_string();
	}
	return "{" + s + "}";
}
