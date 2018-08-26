//
// Created by Chia-Hao on 2018/8/26.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C" {
#include <libavformat/avformat.h>
}

/* Open the files or streaming: rmtp, http, rstp */
bool FFDemux::Open(const char* url)
{
    XLOGI("Open File %s", url);
    int re = avformat_open_input(&ic, url, 0, 0);
    if( re != 0)
    {
        // Will lock mutex
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));

        /* Maybe network failed, not init all, or no permission */
        XLOGE("FFDemux Open %s Failed!", url);
        return false;
    }
    XLOGI("FFDemux Open %s Succeed!", url);

    /* Read file information */
    /* Will fill AVCodecPrameters in */
    re = avformat_find_stream_info(ic, 0);
    if( re != 0)
    {
        // Will lock mutex
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));

        /* Maybe network failed, not init all, or no permission */
        XLOGE("avformat_find_stream_info Failed!", url);
        return false;
    }
    XLOGI("avformat_find_stream_info Succeed!", url);

    this->totalMs = ic->duration/(AV_TIME_BASE/1000); // use micro second
    XLOGI("total %d us", this->totalMs);

    return true;
}

XParameter FFDemux::GetVPara()
{
    if( !ic ) return XParameter();

    int re;
    re = av_find_best_stream( this->ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0 );
    if( re < 0 )
    {
        XLOGE("av_find_best_stream Failed.");
        return XParameter();
    }
    XParameter para;
    para.para = ic->streams[re]->codecpar;
}

/* Read one frame, released by users */
XData FFDemux::Read()
{
    if( !this->ic ) return XData();

    XData d;
    int re;
    AVPacket* packet = av_packet_alloc();
    re = av_read_frame(ic, packet);
    if( re != 0 )
    {
        /* Remember to release packet memory */
        av_packet_free(&packet);
        return XData();
    }
    //XLOGI("Packet Size is %d, PTS is %d", packet->size, packet->pts);

    d.data = (unsigned char*)packet;
    d.size = packet->size;

    return d;
}

FFDemux::FFDemux()
{
    static bool isFirst = true;
    if( isFirst )
    {
        isFirst = false;

        av_register_all();
        /* Register all codec */
        avcodec_register_all();
        /* Initialize Network */
        avformat_network_init();

        XLOGI("Register FFMpeg...");
    }
}