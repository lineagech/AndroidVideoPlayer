//
// Created by Chia-Hao on 2018/8/28.
//
extern "C" {
#include <libavutil/channel_layout.h>
#include <libswresample/swresample.h>
#include <libavcodec/avcodec.h>
}

#include "XLog.h"
#include "FFResample.h"

bool FFResample::Open(XParameter in, XParameter out )
{
    actx = swr_alloc();
    actx = swr_alloc_set_opts(
            actx,
            av_get_default_channel_layout(2 /*ac->channels*/),
            AV_SAMPLE_FMT_S16,
            in.para->sample_rate,
            av_get_default_channel_layout(in.para->channels),
            (AVSampleFormat)in.para->format,
            in.para->sample_rate, 0, 0);
    int re = swr_init(actx);
    if (re != 0) {
        XLOGE("swr_init Failed.");
        return false;
    }
    XLOGI("swr_init Succeeded.");

}