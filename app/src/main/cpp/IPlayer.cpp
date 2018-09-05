//
// Created by Chia-Hao on 2018/8/26.
//

#include "IPlayer.h"
#include "IDemux.h"
#include "IDecode.h"
#include "IAudioPlay.h"
#include "IVideoView.h"
#include "IResample.h"
#include "XLog.h"

IPlayer* IPlayer::Get( unsigned char index )
{
	static IPlayer player[256];
	return &player[index];
}

void IPlayer::Close()
{
	/* Firstly close main and then observers */
	// Sync thread 
	XThread::Stop();
	// Demux
	if( demux )
		demux->Stop();
	// Decode
	if( vdecode )
		vdecode->Stop();
	if( adecode )
		adecode->Stop();

	// Clear buffer queue
	if( vdecode )
		vdecode->Clear();
	if( adecode )
		adecode->Clear();
	if( audioPlay )
		audioPlay->Clear();

	// Clear resource 
	if( audioPlay )
		audioPlay->Close();
	if( videoView )
		videoView->Close();
	if( vdecode )
		vdecode->Close();
	if( adecode )
		adecode->Close();
	if( demux )
		demux->Close();
}

bool IPlayer::Open( const char* path )
{	
	Close();
	mutex.lock();
	if( !demux || !demux->Open(path) ) 
	{	
		mutex.unlock();
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

    outPara = demux->GetAPara();
	if( !resample || !resample->Open(demux->GetAPara(),outPara) )
	{
		XLOGE("resample->Open %s Failed.", path);
	}
	mutex.unlock();
	return true;
}

bool IPlayer::Start()
{	
	mutex.lock();

	if( audioPlay ) audioPlay->StartPlay(outPara);
	
	if( vdecode ) vdecode->Start();

	if( adecode ) adecode->Start();

	if( !demux || !demux->Start() )
	{	
		mutex.unlock();
		XLOGE("demux->Start Failed.");
		return false;
	}

	XThread::Start();

	mutex.unlock();
	return true;
}

void IPlayer::InitView( void* win )
{
	if( videoView )
    {
		videoView->Close();
		videoView->SetRender(win);
	}

}

void IPlayer::Main()
{
	while( !isExit )
	{
		mutex.lock();

		if( !audioPlay || !vdecode )
		{
			mutex.unlock();
			XSleep(2);
			continue;
		}

		/* Synchronization */
		int audio_pts = audioPlay->pts;
		XLOGE("audio pts = %d", audio_pts);

		vdecode->sync_pts = audio_pts;

		mutex.unlock();
		XSleep(2);
	}
}







