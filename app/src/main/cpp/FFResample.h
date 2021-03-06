//
// Created by Chia-Hao on 2018/8/28.
//

#ifndef XPLAY_FFRESAMPLE_H
#define XPLAY_FFRESAMPLE_H


#include "IResample.h"
#include <mutex>

struct SwrContext;

class FFResample : public IResample {
public:
    virtual bool Open( XParameter in, XParameter out=XParameter() );
    virtual void Close();
    virtual XData Resample( XData indata);
protected:
    SwrContext *actx;
    std::mutex mutex;
};


#endif //XPLAY_FFRESAMPLE_H
