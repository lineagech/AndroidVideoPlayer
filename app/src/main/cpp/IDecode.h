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
    virtual bool Open( XParameter para ) = 0;

    /* Future Model, send packet to a thread and return immediately */
    virtual bool SendPacket( XData packet ) = 0;

    /* Get decoding results from queue */
    virtual XData RecvFrame() = 0;

    /* Main entity will notify IDecode when Read done */
    virtual void Update( XData packet );

    bool isAudio = false;

protected:
    virtual void Main();
    std::list<XData> packet_list;

};


#endif //XPLAY_IDECODE_H
