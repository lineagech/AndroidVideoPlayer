//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_IDEMUX_H
#define XPLAY_IDEMUX_H

#include "XData.h"
#include "XThread.h"

/* Unpack Interface */
class IDemux: public XThread {
public:

    /* Open the files or streaming: rmtp, http, rstp */
    virtual bool Open(const char* url) = 0;

    /* Read one frame, released by users */
    virtual XData Read() = 0;

    /* Total time */
    int totalMs = 0;

protected:
    virtual void Main();
};


#endif //XPLAY_IDEMUX_H
