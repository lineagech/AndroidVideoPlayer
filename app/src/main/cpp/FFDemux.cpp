//
// Created by Chia-Hao on 2018/8/26.
//

#include "FFDemux.h"
#include "XLog.h"
extern "C" {
#include <libavformat/avformat.h>
}

// Transfrom to double from rational number
static double r2d( AVRational r)
{
    return ((r.num==0 || r.den == 0) ? 0 : (double)r.num/(double)r.den);
}

void FFDemux::Close()
{
    mutex.lock();
    if( ic ) 
    {
        avformat_close_input(&ic);
    }
    mutex.unlock();
}

/* Open the files or streaming: rmtp, http, rstp */
bool FFDemux::Open(const char* url)
{
    XLOGI("Open File %s", url);

    Close();
    mutex.lock();

    int re = avformat_open_input(&ic, url, 0, 0);
    if( re != 0)
    {
        // Will lock mutex
        mutex.unlock();
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
        mutex.unlock();
        char buf[1024] = {0};
        av_strerror(re, buf, sizeof(buf));

        /* Maybe network failed, not init all, or no permission */
        XLOGE("avformat_find_stream_info Failed!", url);
        return false;
    }
    XLOGI("avformat_find_stream_info Succeed!", url);

    this->totalMs = ic->duration/(AV_TIME_BASE/1000); // use micro second
    mutex.unlock();
    XLOGI("total %d us", this->totalMs);

    /* To get video/audio stream index */
    GetVPara();
    GetAPara();

    return true;
}

XParameter FFDemux::GetVPara()
{
    mutex.lock();
    if( !ic ) {
        mutex.unlock();
        return XParameter();
    }
    int re;
    re = av_find_best_stream( this->ic, AVMEDIA_TYPE_VIDEO, -1, -1, 0, 0 );
    if( re < 0 )
    {   
        mutex.unlock();
        XLOGE("av_find_best_stream Failed.");
        return XParameter();
    }
    videoStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    mutex.unlock();

    return para;
}

XParameter FFDemux::GetAPara()
{
    mutex.lock();
    if( !ic ) {
        mutex.unlock();
        return XParameter();
    }
    int re;
    re = av_find_best_stream( this->ic, AVMEDIA_TYPE_AUDIO, -1, -1, 0, 0 );
    if( re < 0 )
    {   
        mutex.unlock();
        XLOGE("av_find_best_stream Failed.");
        return XParameter();
    }
    audioStream = re;
    XParameter para;
    para.para = ic->streams[re]->codecpar;
    para.channels = ic->streams[re]->codecpar->channels;
    para.sample_rate = ic->streams[re]->codecpar->sample_rate;

    mutex.unlock();
    return para;
}

/* Read one frame, released by users */
XData FFDemux::Read()
{   
    mutex.lock();
    if( !this->ic ) {
        mutex.unlock();
        return XData();
    }
    XData d;
    int re;
    AVPacket* packet = av_packet_alloc();
    re = av_read_frame(ic, packet);
    if( re != 0 )
    {   
        mutex.unlock();
        /* Remember to release packet memory */
        av_packet_free(&packet);
        return XData();
    }
    //XLOGI("Packet Size is %d, PTS is %d", packet->size, packet->pts);

    d.data = (unsigned char*)packet;
    d.size = packet->size;

    if( packet->stream_index == audioStream )
        d.isAudio = true;
    else if( packet->stream_index == videoStream )
        d.isAudio = false;
    else {
        av_packet_free(&packet);
        mutex.unlock();
        return XData();
    }

    // seconds
    packet->pts = packet->pts * r2d(ic->streams[packet->stream_index]->time_base);
    packet->dts = packet->dts * r2d(ic->streams[packet->stream_index]->time_base);
    d.pts = (int)packet->pts;
    //XLOGE("Demux pts %d", d.pts);
    mutex.unlock();

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