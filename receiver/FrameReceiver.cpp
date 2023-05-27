#include "FrameReceiver.hpp"
#include <iostream>

FrameReceiver::FrameReceiver(uint16_t port):receiver{port}{}

void FrameReceiver::start(Callback c){
     receiver.start([this, c](auto data){
          if (data.size() >= sizeof(Packet)){
               auto packet = reinterpret_cast<const Packet*>(data.data()); // if packet is lost we have problem
               bit_rate += packet->part_size; // bitrate
               if (packet->frame_number < current_frame){
                    return;
               }
               if (packet->frame_number > current_frame){
                    if (total_parts >= amount_of_received_parts){
                         amount_of_unreceived_packets += total_parts - amount_of_received_parts; // packet loss
                    }
                    amount_of_received_parts = 0;

                    total_parts = packet->total_parts;
                    summed_parts += total_parts; 

                    current_frame = packet->frame_number; 
                    frame.resize(packet->frame_size);
               }
               std::copy_n(packet->data, packet->part_size, &frame.at(packet->frame_number_part * DATASIZE));
               amount_of_received_parts++;

               if(packet->total_parts == amount_of_received_parts){
                    c(current_frame,frame);
               }
               
          }
    }); 

}

unsigned int FrameReceiver:: get_bit_rate(){
     return bit_rate;
}

unsigned int FrameReceiver:: get_unreceived_packets(){
     return amount_of_unreceived_packets;
}

unsigned int FrameReceiver:: get_summed_parts(){
     return summed_parts;
}


void FrameReceiver:: set_zero(){   
     total_parts = 0;
     bit_rate = 0;
     summed_parts = 0;
}

int FrameReceiver::send( std::span<const char> data){
     return receiver.send(data);

}