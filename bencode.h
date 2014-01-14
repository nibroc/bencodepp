#ifndef TORRENTPP_BENCODE_H
#define TORRENTPP_BENCODE_H

/*
 * bencoding is specified as follows:
 * - Strings are length-prefixed base ten followed by a colon and the string. 
 *   For example 4:spam corresponds to 'spam'.
 * - Integers are represented by an 'i' followed by the number in base 10 
 *   followed by an 'e'. For example i3e corresponds to 3 and i-3e corresponds 
 *   to -3. Integers have no size limitation. i-0e is invalid. All encodings 
 *   with a leading zero, such as i03e, are invalid, other than i0e, which of 
 *   course corresponds to 0.
 * - Lists are encoded as an 'l' followed by their elements (also bencoded) 
 *   followed by an 'e'. For example l4:spam4:eggse corresponds to ['spam', 'eggs'].
 * - Dictionaries are encoded as a 'd' followed by a list of alternating keys 
 *   and their corresponding values followed by an 'e'. For example, 
 *   d3:cow3:moo4:spam4:eggse corresponds to {'cow': 'moo', 'spam': 'eggs'} 
 *   and d4:spaml1:a1:bee corresponds to {'spam': ['a', 'b']}. Keys must be 
 *   strings and appear in sorted order (sorted as raw strings, not alphanumerics).  
*/

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class bencode_value;

std::string bencode(const std::string& str);

std::string bencode(std::int64_t i);

std::string bencode(const std::vector<bencode_value>& list);

std::string bencode(const std::map<bencode_value, bencode_value>& dict);

std::string bencode(const bencode_value& val);

#endif