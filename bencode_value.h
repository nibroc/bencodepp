#ifndef TORRENTPP_BENCODE_VALUE_H
#define	TORRENTPP_BENCODE_VALUE_H

#include "bencode.h"

#include <cstdint>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

class bencode_value {
public:
	enum class type { null, integer, string, list, dict };
	
	typedef std::int64_t int_type;
	typedef std::string string_type;
	typedef std::vector<bencode_value> list_type;
	typedef std::map<bencode_value, bencode_value> dict_type;
	
	bencode_value()
	  : type_(type::null), int_(), string_(), list_(), dict_()
	{ }
	
	explicit bencode_value(std::int64_t v) 
	  : type_(type::integer), int_(v), string_(), list_(), dict_()
	{ }
	
	explicit bencode_value(const std::string& str) 
	  : type_(type::string), int_(), string_(str), list_(), dict_()
	{ }
	
	explicit bencode_value(const list_type& list) 
	  : type_(type::list), int_(), string_(), list_(list), dict_()
	{ }
	
	explicit bencode_value(const dict_type& dict)
	  : type_(type::dict), int_(), string_(), list_(), dict_(dict)
	{ }
	
	type value_type() const { return type_; }
	std::int64_t int_value() const { return int_; }
	std::string string_value() const { return string_; }
	list_type list_value() const { return list_; }
	dict_type dict_value() const { return dict_; }
	
	std::string bencode() const
	{
		switch (type_) {
			case type::integer:
				return ::bencode(int_);
			case type::string:
				return ::bencode(string_);
			case type::list:
				return ::bencode(list_);
			case type::dict:
				return ::bencode(dict_);
			case type::null:
				throw std::runtime_error("Cannot dereference null bencode values");
			default:
				throw std::runtime_error("Unknown bencode_value type");
		}
	}
	
	bool operator<(const bencode_value& other) const
	{
		return bencode() < other.bencode();
	}
	
	std::string to_string() const
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
				throw std::runtime_error("Unknown bencode_value type");
		}
	}
	
	bool operator==(const bencode_value& other)
	{
		if (type_ != other.type_) {
			return false;
		}
		return bencode() == other.bencode();
	}
	
private:
	type type_;
	int_type int_;
	string_type string_;
	list_type list_;
	dict_type dict_;
	
	std::string list_to_string(const list_type& l) const
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
	
	std::string dict_to_string(const dict_type& d) const
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
	
};

#endif
