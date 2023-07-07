#ifndef PACKET_H
#define PACKET_H

#include <span>

namespace ve{
    constexpr size_t DATASIZE = 1400;
    struct Packet {
        unsigned int frame_number{ 0 };
        unsigned int frame_number_part{ 0 };
        unsigned int total_parts{ 0 };
        unsigned int frame_size{ 0 };
        unsigned int part_size{ 0 };
        char data[DATASIZE];
        operator std::span <const char >();
    };
    struct Packet2 {
        float amount_of_frames{ 0 };
        float bit_rate{ 0 };
        float packet_loss{ 0 };
        char button{ '0' };
        float x{ 0 };
        float y{ 0 };
        operator std::span <const char >();


    };
}

#endif