//
// Created by Chia-Hao on 2018/8/26.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main()
{
    for(;;)
    {
        XData d = Read();
        XLOGI("IDemux Read size %d", d.size);
        if( d.size <= 0 ) break;
    }
}