#ifndef BENCODE_VALUE_H
#define	BENCODE_VALUE_H

#include "encode.h"

#include <cstdint>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>

#include <type_traits>
#include <typeinfo>

namespace bencode_detail {
	
	template <std::size_t arg1, std::size_t ... others>
	struct static_max;

	template <std::size_t arg>
	struct static_max<arg> {
		static const size_t value = arg;
	};

	template <std::size_t arg1, std::size_t arg2, std::size_t ... others>
	struct static_max<arg1, arg2, others...> {
		static const std::size_t value = arg1 >= arg2 ? static_max<arg1, others...>::value :
		static_max<arg2, others...>::value;
	};
	
	template<typename ... Types>
	struct union_size {
		static const std::size_t size = static_max<sizeof(Types)...>::value;
	};
	
	template<typename ... Types>
	struct union_alignment {
		static const std::size_t alignment = static_max<alignof(Types)...>::value;
	};
 
	template<std::size_t len, typename ... Types>
	struct aligned_union {
		typedef typename std::aligned_storage<static_max<len, union_size<Types...>::size>::value, 
							union_alignment<Types...>::alignment>::type type;
	};
	
	template<typename ... Types>
	struct minimal_aligned_union {
		typedef typename aligned_union<0, Types...>::type type;
	};
}

namespace bencode {

	class value {
	private:
		template<typename T>
		T& as()
		{
			return *reinterpret_cast<T*>(&storage_);
		}
		
		template<typename T>
		const T& as() const
		{
			return *reinterpret_cast<const T*>(&storage_);
		}
	public:
		enum class type { null = 0, integer, string, list, dict };
	
		typedef std::int64_t int_type;
		typedef std::string string_type;
		typedef std::vector<value> list_type;
		typedef std::map<value, value> dict_type;
		
		value();
		
		explicit value(std::int64_t v);
		
		explicit value(const std::string& str);
		
		explicit value(const list_type& list);
		
		explicit value(const dict_type& dict);
		
		value(const value& v);
		
		~value();
		
		type value_type() const { return type_; }
		
		const std::int64_t& int_value() const { return as<int_type>(); }
		const std::string& string_value() const { return as<string_type>(); }
		const list_type& list_value() const { return as<list_type>(); }
		const dict_type& dict_value() const { return as<dict_type>(); }
		
		std::int64_t& int_value() { return as<int_type>(); }
		std::string& string_value() { return as<string_type>(); }
		list_type& list_value() { return as<list_type>(); }
		dict_type& dict_value() { return as<dict_type>(); }
		
		std::string encode() const;
		
		bool operator<(const value& other) const;
		
		bool operator==(const value& other);
		
		std::string to_string() const;
		
		value& operator=(value v)
		{
			swap(*this, v);
			return *this;
		}
		
	private:
		type type_;
		
		bencode_detail::minimal_aligned_union<int_type, string_type, 
											  list_type, dict_type>::type storage_;
		
		template<typename T>
		void construct(const T& val)
		{
			new (&storage_) T(val);
		}
		
		template<typename T>
		void destruct_type() { reinterpret_cast<T*>(&storage_)->~T(); }
		
		template<typename T>
		void copy_type(const T& val)
		{ construct(val); }
		
		friend void swap(value& first, value& second)
		{
			using std::swap;
			swap(first.type_, second.type_); 
			swap(first.storage_, second.storage_);
		}
		
	};
	
}

#endif
