//
// Created by Chia-Hao on 2018/8/26.
//

#include "IDecode.h"
#include "XLog.h"

void IDecode::Clear()
{
    packet_list_mutex.lock();
    while( !packet_list.empty() )
    {
        packet_list.front().Drop();
        packet_list.pop_front();
    }
    sync_pts = 0;
    curr_pts = 0;
    packet_list_mutex.unlock();
}

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

        /* See if Video/Audio are synchronized */
        if( !isAudio && sync_pts > 0 )
        {
            if( sync_pts < curr_pts ) 
            {
                packet_list_mutex.unlock();
                XSleep(1);
                continue;
            }
        }

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
            while( !isExit ) 
            {
                XData decoded_frame = RecvFrame();
                if( !decoded_frame.data ) break;

                curr_pts = decoded_frame.pts;
                //XLOGI("Recv Frame Size %d", decoded_frame.size);
                /* Notify next observer when decoded frame data is ready */
                this->Notify( decoded_frame );
            }
        }
        recv_packet.Drop();
        packet_list_mutex.unlock();

    }

}