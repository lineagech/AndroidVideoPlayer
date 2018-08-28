//
// Created by Chia-Hao on 2018/8/28.
//

#ifndef XPLAY_FFRESAMPLE_H
#define XPLAY_FFRESAMPLE_H


#include "IResample.h"

struct SwrContext;

class FFResample : public IResample {
public:
    virtual bool Open(XParameter in, XParameter out=XParameter() );

protected:
    SwrContext *actx;
};


#endif //XPLAY_FFRESAMPLE_H
