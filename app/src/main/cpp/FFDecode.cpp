//
// Created by Chia-Hao on 2018/8/26.
//

extern "C" {
#include <libavcodec/avcodec.h>
}
#include "FFDecode.h"
#include "XLog.h"

virtual bool FFDecode::Open( XParameter para )
{
    if( !para.para ) return false;
    AVCodecParameters* p_avcodec_para = para.para;

    AVCodec* codec = avcodec_find_decoder( p_avcodec_para->codec_id );
    if( !codec )
    {
        XLOGE("avcodec_find_decoder Failed, codec id %d", codec->id);
        return false;
    }
    XLOGI("avcodec_find_decoder Succeeded!");

    codec_context = avcodec_alloc_context3( codec );
    avcodec_parameters_to_context( codec_context, p_avcodec_para );

    /* Set thread count as 8 */
    codec_context->thread_count = 8;

    int re = avcodec_open2( codec_context, 0, 0 );
    if( re != 0 )
    {
        char buf[1024] = {0};
        av_strerror( re, buf, sizeof(buf) );
        XLOGE("%s", buf);
        return false;
    }

    if( codec_context->codec_type == AVMEDIA_TYPE_VIDEO )
        this->isAudio = false;
    else
        this->isAudio = true;

    XLOGI("avcodec_open2 Succeeded!");

    return true;
}

bool FFDecode::SendPacket( XData packet )
{
    if( packet.size <= 0 || !packet.data ) return false;
    if( !codec_context ) return false;
    int re;

    re = avcodec_send_packet( codec_context, (AVPacket*)packet.data );
    if( re != 0) return false;

    return true;
}

XData FFDecode::RecvFrame()
{
    if( !codec_context ) return XData();
    if( !frame ) frame = av_frame_alloc();
    XData data;
    int re;

    re = avcodec_receive_frame( codec_context, frame );
    if( re != 0 ) return XData(); // Decoding failed.
    data.data = (unsigned char*) frame;

    if( codec_context->codec_type == AVMEDIA_TYPE_VIDEO )
        data.size = (frame->linesize[0] + frame->linesize[1] + frame->linesize[2]) * frame->height;

    return
}