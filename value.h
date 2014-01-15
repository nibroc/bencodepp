#ifndef TORRENTPP_value_H
#define	TORRENTPP_value_H

#include "encode.h"

#include <cstdint>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace bencode {

	class value {
	public:
		enum class type { null, integer, string, list, dict };
		
		typedef std::int64_t int_type;
		typedef std::string string_type;
		typedef std::vector<value> list_type;
		typedef std::map<value, value> dict_type;
		
		value();
		
		explicit value(std::int64_t v);
		
		explicit value(const std::string& str);
		
		explicit value(const list_type& list);
		
		explicit value(const dict_type& dict);
		
		type value_type() const { return type_; }
		std::int64_t int_value() const { return int_; }
		std::string string_value() const { return string_; }
		list_type list_value() const { return list_; }
		dict_type dict_value() const { return dict_; }
		
		std::string encode() const;
		
		bool operator<(const value& other) const;
		
		bool operator==(const value& other);
		
		std::string to_string() const;
		
	private:
		type type_;
		int_type int_;
		string_type string_;
		list_type list_;
		dict_type dict_;
	};

}

#endif
