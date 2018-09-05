#include "IPlayerBuilder.h"
#include "IVideoView.h"
#include "IDecode.h"
#include "IResample.h"
#include "IAudioPlay.h"
#include "IDemux.h"


IPlayer* IPlayerBuilder::BuilderPlayer( unsigned char index )
{
	IPlayer* player = CreatePlayer(index);

	/* Demux */
    IDemux* de = CreateDemux();

    /* Video Decoding and Audio Decoding */
    IDecode* vdecode = CreateDecode();
    IDecode* adecode = CreateDecode();

    // Send every date to vdecode and adecode
    de->AddObs(vdecode);
    de->AddObs(adecode);

    /* Display Video */
    IVideoView* view = CreateVideoView();
    vdecode->AddObs(view);

    /* Resample Aideo signal */
    IResample* resample = CreateResample();
    adecode->AddObs(resample);

    /* Play Aduio */
    IAudioPlay* audioPlay = CreateAudioPlay();
    resample->AddObs(audioPlay);


    player->demux = de;
    player->adecode = adecode;
    player->vdecode = vdecode;
    player->videoView = view;
    player->resample = resample;
    player->audioPlay = audioPlay;
    
    return player;
}










