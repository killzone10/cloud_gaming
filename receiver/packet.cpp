#include "packet.hpp"

 Packet::operator std::span <const char >(){
        return std::span <const char >{reinterpret_cast<const char*>(this), sizeof(Packet)};
    }


Packet2::operator std::span <const char >(){
        return std::span <const char >{reinterpret_cast<const char*>(this), sizeof(Packet2)};
    }

