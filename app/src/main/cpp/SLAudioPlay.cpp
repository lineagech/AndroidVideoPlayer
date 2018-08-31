//
// Created by Chia-Hao on 2018/8/30.
//

#include "SLAudioPlay.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "XLog.h"

static SLObjectItf engineSL = NULL;
static SLEngineItf eng = NULL;
static SLObjectItf mix = NULL;
static SLObjectItf player = NULL;
static SLPlayItf iplayer = NULL;
static SLAndroidSimpleBufferQueueItf pcmQue =NULL;

SLAudioPlay::SLAudioPlay()
{
    buf = new unsigned char[1024*1024];
}

SLAudioPlay::~SLAudioPlay()
{
    delete buf;
    buf = 0;
}

static SLEngineItf CreateSL()
{
    SLresult re;
    SLEngineItf en;
    re = slCreateEngine(&engineSL,0,0,0,0,0);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->Realize(engineSL,SL_BOOLEAN_FALSE);
    if(re != SL_RESULT_SUCCESS) return NULL;
    re = (*engineSL)->GetInterface(engineSL,SL_IID_ENGINE,&en);
    if(re != SL_RESULT_SUCCESS) return NULL;
    return en;
}

static void PcmCall(SLAndroidSimpleBufferQueueItf bf,void *contex)
{
    SLAudioPlay *ap = (SLAudioPlay *)contex;
    if( !ap )
    {
        XLOGE("PcmCall Failed Contex is NULL.");
        return;
    }
    ap->PlayCall((void *)bf);
}

void SLAudioPlay::PlayCall(void *bufq)
{
    if( !bufq )return;
    SLAndroidSimpleBufferQueueItf bf = (SLAndroidSimpleBufferQueueItf)bufq;

    /* blocking mode */
    XData d = GetData();
    if( d.size <= 0 )
    {
        XLOGE("GetData() Size is 0... ");
        return;
    }
    if( !buf ) return;
    memcpy(buf,d.data,d.size);
    (*bf)->Enqueue(bf,buf,d.size);
    d.Drop();
}

bool SLAudioPlay::StartPlay(XParameter out)
{
    /* Create Engine */
    eng = CreateSL();
    if( eng )
    {
        XLOGI("CreateSL success！ ");
    }
    else
    {
        XLOGE("CreateSL failed！ ");
        return false;
    }

    /* Ouput Mix */
    SLresult re = 0;
    re = (*eng)->CreateOutputMix(eng,&mix,0,0,0);
    if( re !=SL_RESULT_SUCCESS )
    {
        XLOGE("SL_RESULT_SUCCESS failed!");
        return false;
    }
    re = (*mix)->Realize(mix,SL_BOOLEAN_FALSE);
    if( re !=SL_RESULT_SUCCESS )
    {
        XLOGE("(*mix)->Realize failed!");
        return false;
    }
    SLDataLocator_OutputMix outmix = {SL_DATALOCATOR_OUTPUTMIX,mix};
    SLDataSink audioSink= {&outmix,0};

    /* Buffer Queue */
    SLDataLocator_AndroidSimpleBufferQueue que = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,10};
    /* Pulse Coded Modulation Format */
    SLDataFormat_PCM pcm = {
            SL_DATAFORMAT_PCM,
            (SLuint32) out.channels,
            (SLuint32) out.sample_rate*1000,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT|SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };
    SLDataSource ds = {&que,&pcm};


    /* Create Player */
    const SLInterfaceID ids[] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[] = {SL_BOOLEAN_TRUE};
    re = (*eng)->CreateAudioPlayer(eng,&player,&ds,&audioSink,sizeof(ids)/sizeof(SLInterfaceID),ids,req);
    if(re !=SL_RESULT_SUCCESS )
    {
        XLOGE("CreateAudioPlayer Failed!");
        return false;
    } else{
        XLOGI("CreateAudioPlayer Succeeded!");
    }
    (*player)->Realize(player,SL_BOOLEAN_FALSE);

    /* Get player interface */
    re = (*player)->GetInterface(player,SL_IID_PLAY,&iplayer);
    if(re !=SL_RESULT_SUCCESS )
    {
        XLOGE("GetInterface SL_IID_PLAY Failed!");
        return false;
    }
    re = (*player)->GetInterface(player,SL_IID_BUFFERQUEUE,&pcmQue);
    if(re !=SL_RESULT_SUCCESS )
    {
        XLOGE("GetInterface SL_IID_BUFFERQUEUE Failed!");
        return false;
    }

    /* Register Callback function */
    (*pcmQue)->RegisterCallback(pcmQue,PcmCall,this);

    /* Setting playing state */
    (*iplayer)->SetPlayState(iplayer,SL_PLAYSTATE_PLAYING);

    /* Enable it */
    (*pcmQue)->Enqueue(pcmQue,"",1);
    XLOGI("SLAudioPlay::StartPlay Succeeded.");
    return true;
}