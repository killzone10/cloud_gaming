#include "packet.hpp"
#include "socket.hpp"
#include "receiver.hpp"

#include <span>
#include <algorithm>


class FrameReceiver{
    private:
        Receiver receiver;
        std::vector <char> frame;
        unsigned int current_frame{0};
        unsigned int amount_of_received_parts{0};
        unsigned int amount_of_unreceived_packets{0};
        unsigned int bit_rate{0};
        unsigned int total_parts {0};
        unsigned int summed_parts {0};

    public:
        using Callback = std::function <void(unsigned int frame_number, std::span<const char>)>;
        FrameReceiver(uint16_t port);
        void start(Callback c);
        unsigned int get_bit_rate();
        unsigned int get_unreceived_packets();
        unsigned int get_summed_parts();

        void set_zero();
        int send(std::span<const char>data);
      
};