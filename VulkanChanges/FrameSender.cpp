#include "FrameSender.h"
namespace ve {
    FrameSender::FrameSender() :sender{ 0 } {
    }

    void FrameSender::send(unsigned int frameNumber, std::span<const char> data) {
        const unsigned int division = (data.size() - 1 + DATASIZE) / DATASIZE;
        size_t offset{ 0 };
        unsigned int i{ 0 };
        Packet p;
        std::span<const char> part;
        while (offset < data.size()) {
            if (data.size() - offset < DATASIZE) {
                part = data.subspan(offset, data.size() - offset); //1400
            }
            else {
                part = data.subspan(offset, DATASIZE); // <1400
            }
            p.frame_number = frameNumber;
            p.frame_number_part = i;
            p.frame_size = data.size();
            p.part_size = part.size();
            p.total_parts = division;
            std::copy_n(part.data(), part.size(), p.data);
            offset += DATASIZE;
            i++;
            sender.send(p);
        }

    }

    void FrameSender::connect(const std::string& name, uint16_t port) {
        sender.connect(name, port);
    }

    std::optional<std::span<const char>> FrameSender::receive() {
        return sender.receive();
    }
}