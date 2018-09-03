//
// Created by Chia-Hao on 2018/8/26.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IAudioPlay.h"
#include "IVideoView.h"

IPlayer* IPlayer::Get( unsigned char index=0 )
{
	static IPlayer player[256];
	return player[index];
}

bool IPlayer::Open( const char* patch )
{	
	if( !demux || !demux->Open(path) ) 
	{
		XLOGE("demux->Open %s Failed.", path);
		return false;
	}
	if( !vdecode || !vdecode->Open(demux->GetVPara(), isHardDecode) ) 
	{
		XLOGE("vdecode->Open %s Failed.", path);
		/* If data is raw, do not need to decode */
		//return false;
	}
	if( !adecode || !adecode->Open(demux->GetAPara()) ) 
	{
		XLOGE("adecode->Open %s Failed.", path);
		/* If data is raw, do not need to decode */
		//return false;
	}
	if( outPara.sample_rate <= 0 )
		outPara = demux->GetAPara();
	if( !resample || resample->Open( demux->GetAPara() ,outPara) )
	{
		XLOGE("resample->Open %s Failed.", path);
	}

	return true;
}

bool IPlayer::Start( )
{	
	if( !demux || !demux->Start() )
	{
		XLOGE("demux->Start Failed.");
		return false;
	}

	if( adecode ) adecode->Start();

	if( audioPlay ) audioPlay->StartPlay(outPara);

	if( vdecode ) vdecode->Start();

	return true;
}

void IPlayer::InitView( void* win )
{
	if( videoView ) 
		videoView->SetRender(win);
}

virtual void IPlayer::InitHard(void* vm)
{
	
}







