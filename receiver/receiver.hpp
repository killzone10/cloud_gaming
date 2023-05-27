#pragma once
#include "socket.hpp"
#include <thread>
#include <functional>
#include <atomic>


class Receiver{
    private:
        Socket receiver;
        std::thread t_receiver;
        std::atomic<bool>end{false};
    public:
        explicit Receiver(uint16_t port);
        void start(std::function <void(std::span<const char>)> call_back);
        ~Receiver();

        int send(std::span <const char >data);
        
};