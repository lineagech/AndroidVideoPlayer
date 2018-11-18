//
// Created by Chia-Hao on 2018/8/28.
//

#include "IResample.h"
#include "XLog.h"

void IResample::Update(XData data)
{
    XData d = this->Resample(data);
    //XLOGE("Resample::Update");
    if( d.size > 0 ) {
        this->Notify(d);
    }
}

