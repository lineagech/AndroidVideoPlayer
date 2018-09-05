#include "FFPlayerBuilder.h"
#include "FFDemux.h"
#include "FFDecode.h"
#include "FFResample.h"
#include "GLVideoView.h"
#include "SLAudioPlay.h"

IDemux* FFPlayerBuilder::CreateDemux()
{
	IDemux* ff_demux = new FFDemux();
	return ff_demux;
}
IDecode* FFPlayerBuilder::CreateDecode()
{
	IDecode* ff_decode = new FFDecode();
	return ff_decode;
}
IResample* FFPlayerBuilder::CreateResample()
{
	IResample* ff_resample = new FFResample();
	return ff_resample;
}
IVideoView* FFPlayerBuilder::CreateVideoView()
{
	IVideoView* ff_videoview = new GLVideoView();
	return ff_videoview;
}
IAudioPlay* FFPlayerBuilder::CreateAudioPlay()
{
	IAudioPlay* ff_audioplayer = new SLAudioPlay();
	return ff_audioplayer;
}
IPlayer* FFPlayerBuilder::CreatePlayer( unsigned char index )
{
	return IPlayer::Get(index);
}

void FFPlayerBuilder::InitHard(void* vm)
{
	FFDecode::InitHard(vm);
}