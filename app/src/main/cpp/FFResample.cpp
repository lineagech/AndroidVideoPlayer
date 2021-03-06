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

void FFResample::Close()
{
    mutex.lock();
    if( actx ) swr_free(&actx);
    mutex.unlock();
}

bool FFResample::Open( XParameter in, XParameter out )
{
    Close();
    mutex.lock();
    actx = swr_alloc();
    actx = swr_alloc_set_opts(
            actx,
            av_get_default_channel_layout(out.channels),
            AV_SAMPLE_FMT_S16,
            out.sample_rate,
            av_get_default_channel_layout(in.para->channels),
            (AVSampleFormat)in.para->format,
            in.para->sample_rate, 0, 0);
    int re = swr_init(actx);
    if (re != 0) {
        mutex.unlock();
        XLOGE("swr_init Failed.");
        return false;
    }
    XLOGI("swr_init Succeeded.");

    outChannels = in.para->channels;
    outFormat = AV_SAMPLE_FMT_S16;

    mutex.unlock();
    return true;
}

XData FFResample::Resample( XData indata ) 
{   
    if( indata.size <= 0 || !indata.data ) return XData();
    mutex.lock();
    if( !actx ) {
        mutex.unlock();
        return XData();
    }
    //XLOGE("indata size is %d", indata.size);

    /*  Allocate Memory */
    XData out;
    AVFrame* frame = (AVFrame* )indata.data;
    //out.Alloc();

    /* Calculate size */
    int size = outChannels * frame->nb_samples * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    if( size <= 0 ) return XData();

    if(!out.Alloc(size)) {
        mutex.unlock();
        XLOGE("XData Alloc Failed!");
        return XData();
    }
    uint8_t* outArr[2] = {0};
    outArr[0] = out.data;
    int len = swr_convert(actx, outArr, frame->nb_samples, (const uint8_t**)frame->data, frame->nb_samples );

    if( len <= 0) {
        mutex.unlock();
        out.Drop();
        return XData();
    }
    //XLOGE("swr_convert Succeeded, %d", len);

    out.pts = indata.pts;
    mutex.unlock();

    return out;
}