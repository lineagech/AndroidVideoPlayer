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

    int re = avcodec_open2( codec_context, 0, 0 );
    if( re != 0 )
    {
        char buf[1024] = {0};
        av_strerror( re, buf, sizeof(buf) );
        XLOGE("%s", buf);
        return false;
    }
    XLOGI("avcodec_open2 Succeeded!");

    return true;
}