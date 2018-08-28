//
// Created by Chia-Hao on 2018/8/28.
//

#ifndef XPLAY_IRESAMPLE_H
#define XPLAY_IRESAMPLE_H


#include "IObserver.h"
#include "XParameter.h"

class IResample : IObserver
{
public:
    virtual bool Open(XParameter in, XParameter out=XParameter() ) = 0;
};


#endif //XPLAY_IRESAMPLE_H
