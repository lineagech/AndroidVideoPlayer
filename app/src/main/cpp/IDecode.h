//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_IDECODE_H
#define XPLAY_IDECODE_H


#include "XParameter.h"
#include "IObserver.h"
#include <list>

/* Decode Interface */
class IDecode : public IObserver
{
public:
    /* open decoder */
    virtual bool Open( XParameter para, bool isHard=false ) = 0;
    virtual void Close() = 0;
    virtual void Clear();

    /* Future Model, send packet to a thread and return immediately */
    virtual bool SendPacket( XData packet ) = 0;

    /* Get decoding results from queue */
    virtual XData RecvFrame() = 0;

    /* Main entity will notify IDecode when Read done */
    virtual void Update( XData packet );

    bool isAudio = false;
    int max_packet_list_size = 100; //around 4 sec duration buffer

    /* Synchronization Time, if reopening the file, need to clear */
    int sync_pts = 0; // from audio decoding result, will compare to curr_ptsz
    int curr_pts = 0;

protected:
    virtual void Main();
    std::list<XData> packet_list;
    std::mutex       packet_list_mutex;

};


#endif //XPLAY_IDECODE_H
