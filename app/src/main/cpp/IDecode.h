//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_IDECODE_H
#define XPLAY_IDECODE_H


#include "XParameter.h"
#include "IObserver.h"

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
};


#endif //XPLAY_IDECODE_H
