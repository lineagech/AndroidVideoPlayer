#include "FFPlayerBuilder.h"

IDemux* FFPlayerBuilder::CreateDemux()
{
	IDemux* ff_demux = new FFDemux();
	return ff_demux;
}
IDecode* FFPlayerBuilder::CreateDecode()
{
	IDemux* ff_decode = new FFDecode();
	return ff_decode;
}
IResample* FFPlayerBuilder::CreateResample()
{
	IDemux* ff_resample = new FFResample();
	return ff_resample;
}
IVideoView* FFPlayerBuilder::CreateVideoView()
{
	IDemux* ff_videoview = new GLVideoView();
	return ff_videoview;
}
IAudioPlay* FFPlayerBuilder::CreateAudioPlay()
{
	IDemux* ff_audioplayer = new SLAudioPlay();
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