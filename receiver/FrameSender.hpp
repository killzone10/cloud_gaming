#include "packet.hpp"
#include "socket.hpp"
#include <span>
#include <algorithm>


class FrameSender{
    private:
        Socket sender;
    public:
        FrameSender();
        void send(unsigned int frameNumber, std::span<const char> data);
        void connect(const std::string &name, uint16_t port);
        
};