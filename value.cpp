#include "value.h"

#include <stdexcept>

static std::string list_to_string(const bencode::value::list_type& l);

static std::string dict_to_string(const bencode::value::dict_type& d);

namespace bencode {

value::value()
  : type_(type::null), int_(), string_(), list_(), dict_()
{ }

value::value(std::int64_t v) 
  : type_(type::integer), int_(v), string_(), list_(), dict_()
{ }

value::value(const std::string& str) 
  : type_(type::string), int_(), string_(str), list_(), dict_()
{ }

value::value(const list_type& list) 
  : type_(type::list), int_(), string_(), list_(list), dict_()
{ }

value::value(const dict_type& dict)
  : type_(type::dict), int_(), string_(), list_(), dict_(dict)
{ }

std::string value::encode() const
{
	switch (type_) {
		case type::integer:
			return bencode::encode(int_);
		case type::string:
			return bencode::encode(string_);
		case type::list:
			return bencode::encode(list_);
		case type::dict:
			return bencode::encode(dict_);
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
			return std::to_string(int_);
		case type::string:
			return string_;
		case type::list:
			return list_to_string(list_);
		case type::dict:
			return dict_to_string(dict_);
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
