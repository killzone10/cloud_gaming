#ifndef FRAMESENDER_H
#define FRAMESENDER_H

#include "packet.h"
#include "socket.h"
#include <span>

namespace ve {
    class FrameSender {
        private:
            Socket sender;
        public:
            FrameSender();
            void send(unsigned int frameNumber, std::span<const char> data);
            void connect(const std::string& name, uint16_t port);
            std::optional<std::span<const char>> receive(); // can return span (pointer) or nothing if nothing - failed

    };
}

#endif