#include "socket.hpp"
#include "receiver.hpp"
#include "packet.hpp"
#include "FrameReceiver.hpp"
#include "decoder.hpp"
#include "SafeQ.hpp"
#include <iostream>
#include <thread>
#include <random>
#include <chrono>
#include <complex>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>
#include <sdl2/SDL.h>



/*Threads A does the following:

It waits on the UDP socket to receive packets. 
After receiving all fragments (is done automatically in IPv6) the frame is decoded.
The decoded frame is put into a FIFO-queue. Use STL for locks and the queue.
Every 10 seconds or so send a receiver report to the sender, holding the following information: 
     Received byte rate
     Packet loss rate
     Frame rate
*/
static std::atomic<bool> running = true;

int producer(SafeQ<AVFrame*> &q, std::atomic<unsigned int> &amount_of_frames, FrameReceiver &s2){ // decoding /*

    Decoder decoder;
    s2.start([&decoder, &q]( unsigned int frame_number, std::span<const char> frame){
        AVFrame* finalFrame;
        finalFrame = decoder.initDecode(frame);
        q.push(finalFrame);
    });

    while(running){
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::cout <<"Amount of frames" << amount_of_frames << " Bit Rate: "<< s2.get_bit_rate() 
                  <<"Total Parts"<< s2.get_summed_parts() << " Unreceived packets: "
                  <<s2.get_unreceived_packets()<< std::endl;
        Packet2 packet;
        packet.amount_of_frames = static_cast<float>(amount_of_frames)/10.0;
        packet.bit_rate = static_cast<float>(s2.get_bit_rate())/10.0;
        packet.packet_loss = static_cast<float>((s2.get_unreceived_packets())/static_cast<float>(s2.get_summed_parts()));
        s2.send(packet);
        amount_of_frames = 0;
        s2.set_zero();

    }

    return 0;
}




int main(int argc, char* argv[]){

    if( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		throw std::runtime_error{SDL_GetError()};
    }
    
    std::atomic<unsigned int> amount_of_frames{0};
    SDL_Renderer *sdlRenderer{nullptr};
    

    SDL_Window *sdlWindow = SDL_CreateWindow("My Game Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          800, 600,
                          SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

   

    if (sdlWindow == NULL){
        std::cout<<"window creation error"<<std::endl;
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (sdlRenderer == NULL){
        std::cout<<"renderer creation error"<<std::endl;
    }

    SDL_Texture *sdlTexture = SDL_CreateTexture(sdlRenderer,
        SDL_PIXELFORMAT_YV12, //SDL_PIXELFORMAT_IYUV moze byc to albo to
        SDL_TEXTUREACCESS_STREAMING,
        800, 600);
        
    if (sdlTexture == NULL){
        std::cout<<"texture creation error"<<std::endl;
    }


    SafeQ<AVFrame*> q;
    std::vector<std::thread> workers;
    int i = 0;
    //auto prod = std::async(std::launch::async, producer, std::ref(q), std::ref(port));
    FrameReceiver s2{9010};

    std::thread prod{producer, std::ref(q), std::ref(amount_of_frames), std::ref(s2)};
    bool keys[SDL_NUM_SCANCODES] = { false };

    while (running){ 
        SDL_Event event{};

        while (SDL_PollEvent(&event)) {
            Packet2 packet;
            packet.amount_of_frames = static_cast<float>(amount_of_frames)/10.0;
            packet.bit_rate = static_cast<float>(s2.get_bit_rate())/10.0;
            packet.packet_loss = static_cast<float>((s2.get_unreceived_packets())/static_cast<float>(s2.get_summed_parts()));
            /* handle your event here */ 
            if( event.type == SDL_QUIT ){
                running = false;
            }
            else if (event.type == SDL_KEYDOWN){
                 keys[event.key.keysym.scancode] = true;
                 
            }
            else if (event.type == SDL_KEYUP) {
                keys[event.key.keysym.scancode] = false;
            }
            else if (event.type == SDL_MOUSEMOTION){
                packet.button = '2';

                int w, h;
                SDL_GetWindowSize(sdlWindow, &w, &h);
                packet.x = event.motion.x;
                packet.y = event.motion.y;
                // packet.x = static_cast<float>(event.motion.x) / w;
                // packet.y = static_cast<float>(event.motion.y) / h;
                s2.send(packet);


            }
            else if (event.type == SDL_MOUSEBUTTONDOWN){
                packet.button = '3';
               
                int w, h;
                SDL_GetWindowSize(sdlWindow, &w, &h);
                packet.x = event.motion.x;
                packet.y = event.motion.y;
                // packet.x = static_cast<float>(event.motion.x) / w;
                // packet.y = static_cast<float>(event.motion.y) / h;
                s2.send(packet);


            }
             else if (event.type == SDL_MOUSEBUTTONUP){
                packet.button = '4';
               
                int w, h;
                SDL_GetWindowSize(sdlWindow, &w, &h);
                packet.x = event.motion.x;
                packet.y = event.motion.y;
                // packet.x = static_cast<float>(event.motion.x) / w;
                // packet.y = static_cast<float>(event.motion.y) / h;
                s2.send(packet);


            }

            }
        Packet2 packet;
        packet.amount_of_frames = static_cast<float>(amount_of_frames)/10.0;
        packet.bit_rate = static_cast<float>(s2.get_bit_rate())/10.0;
        packet.packet_loss = static_cast<float>((s2.get_unreceived_packets())/static_cast<float>(s2.get_summed_parts()));

        if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A]) {
            packet.button = 'w';
            s2.send(packet);
            packet.button = 'a';
            s2.send(packet);
        }

        else if (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_D]) {
            packet.button = 'w';
            s2.send(packet);
            packet.button = 'd';
            s2.send(packet);
        }

        else if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_A]) {
            packet.button = 's';
            s2.send(packet);
            packet.button = 'a';
            s2.send(packet);
        }

        else if (keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D]) {
            packet.button = 's';
            s2.send(packet);
            packet.button = 'd';
            s2.send(packet);
        }
        else if (keys[SDL_SCANCODE_W]) {
            packet.button = 'w';
            s2.send(packet);
        }

        else if (keys[SDL_SCANCODE_A]) {
            packet.button = 'a';
            s2.send(packet);
        }

        else if (keys[SDL_SCANCODE_S]) {
            packet.button = 's';
            s2.send(packet);
        }

        else if (keys[SDL_SCANCODE_D]) {
            packet.button = 'd';
            s2.send(packet);
        }
         else if (keys[SDL_SCANCODE_R]) {
            packet.button = 'r';
            s2.send(packet);
        }
        else if (keys[SDL_SCANCODE_ESCAPE]){
            packet.button = '9';
            s2.send(packet);
            running = false;


        }
        AVFrame * frame ={nullptr};
        q.pop(frame);
        if (frame != nullptr) {
            SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
            SDL_RenderClear(sdlRenderer);
            
            SDL_UpdateYUVTexture(sdlTexture, NULL, frame->data[0], frame->linesize[0],
                                frame->data[1], frame->linesize[1],
                                frame->data[2], frame->linesize[2]); 
            amount_of_frames++;
            SDL_RenderCopy(sdlRenderer, sdlTexture, nullptr, nullptr);
            SDL_RenderPresent(sdlRenderer);

        }
        else {
            SDL_Delay(1000 / 60);
        } 

        
    }   

    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
    prod.join();

    return 0;

}
