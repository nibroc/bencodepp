#ifndef BENCODE_DECODE_H
#define	BENCODE_DECODE_H

#include "value.h"

namespace bencode {
	value decode(const std::string& str);
}

#endif
