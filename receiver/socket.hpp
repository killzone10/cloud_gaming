#pragma once
#include<stdio.h>
#include<winsock2.h>
#include <stdexcept>
#include <string>
#include <span>
#include <vector>
#include <optional>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

class Socket {
    private:
        static void init();
        static bool checkInit;
        SOCKET descryptor{INVALID_SOCKET};
        std::vector<char>buffer;
        sockaddr address{};

    public:
        Socket(uint16_t port);
        ~Socket();
        Socket(const Socket&) = delete; // remove copy constructor
        Socket& operator = (const Socket&) = delete;
        Socket(Socket &&) = default;
        Socket& operator = (Socket&&) = default;
        int send(std::span <const char >data);
        std::optional<std::span<const char>> receive(); // can return span (pointer) or nothing if nothing - failed
        void connect(const sockaddr *name);
        void connect(const std::string &name, uint16_t port);
        SOCKET getSocket();
        sockaddr* get_addr();

};