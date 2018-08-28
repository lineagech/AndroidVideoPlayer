//
// Created by Chia-Hao on 2018/8/26.
//

#include "IDecode.h"
#include "XLog.h"

void IDecode::Update( XData packet )
{
    if( packet.isAudio != isAudio ) return;

    while( !isExit )
    {
        packet_list_mutex.lock();

        if (packet_list.size() < max_packet_list_size) {
            packet_list.push_back(packet);
            packet_list_mutex.unlock();
            break;
        }

        packet_list_mutex.unlock();
        XSleep(1); // Blocking mode

        //XLOGE("Main Thread %d Update", isAudio);
    }
}

void IDecode::Main()
{
    while( !isExit )
    {
        packet_list_mutex.lock();

        if( packet_list.empty() )
        {
            packet_list_mutex.unlock();
            XSleep(1); // Release CPU resource
            continue;
        }
        /* Get a packet from packet list */
        XData recv_packet = packet_list.front();
        packet_list.pop_front();
        if( this->SendPacket( recv_packet ) )
        {
            /* One packet sent can result in multiple decoded frames, using while to handle that */
            while( !isExit ) {
                XData decoded_frame = RecvFrame();
                if( !decoded_frame.data ) break;
                //XLOGI("Recv Frame Size %d", decoded_frame.size);
                /* Notify next observer when decoded frame data is ready */
                this->Notify( decoded_frame );
            }
        }
        recv_packet.Drop();
        packet_list_mutex.unlock();

    }

}