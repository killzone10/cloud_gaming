#include "socket.h"

namespace ve {

    bool Socket::checkInit{ false };

    void Socket::init() {
        WSADATA wsa;
        if (checkInit) {
            return;
        }
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            throw std::runtime_error("WSAStartup failed in constructor");
        }
        checkInit = true;
    }

    Socket::Socket(uint16_t port) {
        Socket::init();
        descryptor = socket(AF_INET, SOCK_DGRAM, 0);
        if (descryptor == INVALID_SOCKET) {
            throw std::runtime_error("Descryptor equals INVALID SOCKET");
        }

        sockaddr_in addr_in{}; // init to 0
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(port); // little endian vs big endian
        addr_in.sin_addr.s_addr = INADDR_ANY;

        if (bind(descryptor, reinterpret_cast<sockaddr*>(&addr_in), sizeof(addr_in)) == SOCKET_ERROR) {
            throw std::runtime_error("BIND method returner error");

        }

        buffer.resize(2000);

    }


    Socket :: ~Socket() {
        if (descryptor != INVALID_SOCKET) {
            closesocket(descryptor);
        }

    }

    void Socket::connect(const sockaddr* name) {
        if (::connect(descryptor, name, sizeof(sockaddr)) == SOCKET_ERROR) {
            throw std::runtime_error("Socket connection error");
        }

    }

    void Socket::connect(const std::string& name, uint16_t port) {
        sockaddr_in addr_in{}; // init to 0
        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(port); // little endian vs big endian
        addr_in.sin_addr.s_addr = inet_addr(name.c_str());
        connect(reinterpret_cast<sockaddr*>(&addr_in));
    }
    int Socket::send(std::span <const char >data) {
        if (::send(descryptor, data.data(), data.size(), 0) < 0)
        {
            return 1;
        }
        return 0;
    }

    std::optional<std::span<const char>> Socket::receive() {
        int recv_size{ SOCKET_ERROR };
        if ((recv_size = recv(descryptor, buffer.data(), buffer.size(), 0)) == SOCKET_ERROR) {

            return std::nullopt;
        }

        return buffer;
    }


    SOCKET Socket::getSocket() {
        return descryptor;
    }
}