#include "receiver.hpp"
#include <iostream>
Receiver::Receiver(uint16_t port):receiver{port}{
}

void Receiver::start(std::function <void(std::span<const char>)> call_back){
    t_receiver = std::thread([this, call_back](){
    bool done = false;
    pollfd fdArray{};
    fdArray.fd = receiver.getSocket();
    fdArray.events = POLLIN ;
    while(!end){
        const int a = WSAPoll(&fdArray, 1, 10); //waitin here
        if (a == SOCKET_ERROR){
            std::cout << "Receiver::start Socket Error"<<std::endl;
        }
        else if (a > 0){
            auto data = receiver.receive();
            if (!done){
                receiver.connect(receiver.get_addr());
                done = true;
            }
            if (data.has_value()){
                call_back(*data);

            }
        }

    }
       

    });
}

Receiver::~Receiver(){
    end = true;
    if (t_receiver.joinable()){
        t_receiver.join();
    }
}

int Receiver::send(std::span <const char >data){
    return receiver.send(data);
}