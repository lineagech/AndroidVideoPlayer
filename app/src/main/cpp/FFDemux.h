//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_FFDEMUX_H
#define XPLAY_FFDEMUX_H


#include "IDemux.h"

struct AVFormatContext;

class FFDemux: public IDemux {
public:
    FFDemux();
    /* Open the files or streaming: rmtp, http, rstp */
    virtual bool Open(const char* url);

    virtual XParameter GetVPara();

    /* Read one frame, released by users */
    virtual XData Read();

private:
    AVFormatContext* ic = NULL;
};


#endif //XPLAY_FFDEMUX_H
