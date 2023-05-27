#include "packet.h"

namespace ve {
	Packet::operator std::span <const char >() {
		return std::span <const char >{reinterpret_cast<const char*>(this), sizeof(Packet)};
	}

}


