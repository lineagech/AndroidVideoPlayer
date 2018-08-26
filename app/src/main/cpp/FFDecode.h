//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_FFDECODE_H
#define XPLAY_FFDECODE_H


#include "XParameter.h"
#include "IDecode.h"

struct AVCodecContext;

class FFDecode: public IDecode {
public:
    virtual bool Open( XParameter para );

protected:
    AVCodecContext* codec_context = 0;
};


#endif //XPLAY_FFDECODE_H
