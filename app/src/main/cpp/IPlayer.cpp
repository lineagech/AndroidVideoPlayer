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
    if( audioPlay )
        audioPlay->Stop();

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
    XLOGE("IPlayer->Start");

	if( vdecode ) vdecode->Start();
	XLOGE("vdecode->Start");

	if( !demux || !demux->Start() )
	{	
		mutex.unlock();
		XLOGE("demux->Start Failed.");
		return false;
	}
    XLOGE("demux->Start");

    if( adecode ) adecode->Start();
    XLOGE("adecode->Start");

    if( audioPlay ) audioPlay->StartPlay(outPara);
    XLOGE("audioPlay->Start");

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

void IPlayer::Pause( bool pause_or_not )
{
    mutex.lock();

    XThread::Pause(pause_or_not);
    if( demux )
        demux->Pause( pause_or_not );
    if( vdecode )
        vdecode->Pause( pause_or_not );
    if( adecode )
        adecode->Pause( pause_or_not );
    if( audioPlay )
        audioPlay->Pause( pause_or_not );

    mutex.unlock();
}

double IPlayer::curr_playing_position()
{
    mutex.lock();

    double curr_pos = 0.0;
    if( demux && vdecode && demux->totalMs )
    {
        //XLOGE("curr_playing_position %d, %d", vdecode->curr_pts, demux->totalMs);
        curr_pos = (double)vdecode->curr_pts / (double)demux->totalMs;
    }

    mutex.unlock();
    return curr_pos;
}

void IPlayer::seek_update_progress( double progress )
{
    if( !demux ) return;

    Pause(true);
    mutex.lock();
    /* clear queue's data */
    if( vdecode ){
        vdecode->Clear();
    }
    if( adecode ){
        adecode->Clear();
    }
    if( audioPlay ){
        audioPlay->Clear();
    }
    demux->Seek( progress );
    if( !vdecode )
    {
        mutex.unlock();
        Pause(false);
        return;
    }

    /* Transform to pts from progress */
    int updated_ms = progress * demux->totalMs;

    while( !isExit )
    {
        XData read_data = demux->Read();
        if( read_data.size <= 0 ) break;

        if( read_data.isAudio )
        {
            if( read_data.pts < updated_ms ) {
                read_data.Drop();
                continue;
            }
            demux->Notify( read_data );
            continue;
        }
        else
        {
            int re = vdecode->SendPacket( read_data );
            read_data.Drop();

            XData decoded_video_data =  vdecode->RecvFrame();
            if( decoded_video_data.size <= 0 ) continue;
            if( decoded_video_data.pts >= updated_ms )
            {
                //vdecode->Notify( decoded_video_data );
                break;
            }
        }
    }

    mutex.unlock();
    Pause(false);
}

void IPlayer::Main()
{
    XLOGE("IPlayer Main()");
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
		//XLOGE("audio pts = %d", audio_pts);

		vdecode->sync_pts = audio_pts;

		mutex.unlock();
		XSleep(2);
	}
}







