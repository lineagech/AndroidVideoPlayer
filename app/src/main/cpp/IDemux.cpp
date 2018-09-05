//
// Created by Chia-Hao on 2018/8/26.
//

#include "IDemux.h"
#include "XLog.h"

void IDemux::Main()
{
    while( !isExit )
    {
        if( isPause() )
        {
            XSleep(2);
            continue;
        }
        XData d = Read();
        if( d.size > 0 ) {
            Notify(d); // Blocking mode, if packet list is full
        }
        else{
            XSleep(1);
        }
    }
}