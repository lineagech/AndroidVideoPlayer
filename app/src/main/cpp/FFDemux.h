//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_FFDEMUX_H
#define XPLAY_FFDEMUX_H


#include "IDemux.h"
#include <mutex>

struct AVFormatContext;

class FFDemux: public IDemux {
public:
    FFDemux();
    /* Open the files or streaming: rmtp, http, rstp */
    virtual bool Open(const char* url);
    virtual void Close();
    virtual void Seek(double pos);

    virtual XParameter GetVPara();
    virtual XParameter GetAPara();

    /* Read one frame, released by users */
    virtual XData Read();

private:
    AVFormatContext* ic = NULL;
    std::mutex mutex;
    int audioStream = 1;
    int videoStream = 0;

};


#endif //XPLAY_FFDEMUX_H
