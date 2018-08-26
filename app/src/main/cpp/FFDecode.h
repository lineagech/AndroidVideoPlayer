//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_FFDECODE_H
#define XPLAY_FFDECODE_H


#include "XParameter.h"
#include "IDecode.h"

struct AVCodecContext;
struct AVFrame;

class FFDecode: public IDecode {
public:
    virtual bool Open( XParameter para );

    /* Future Model, send packet to a thread and return immediately */
    virtual bool SendPacket( XData packet );

    /* Get decoding results from queue */
    virtual XData RecvFrame();

protected:
    AVCodecContext* codec_context = 0;
    AVFrame* frame = 0;
};


#endif //XPLAY_FFDECODE_H