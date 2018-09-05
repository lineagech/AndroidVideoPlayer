
#ifndef XPLAY_IPLAYER_H
#define XPLAY_IPLAYER_H

#include <mutex>
#include "XThread.h"
#include "XParameter.h"

class IDemux; /* declare first so that do not need to include header file */
class IDecode;
class IVideoView;
class IAudioPlay;
class IResample;

/* Fascade Pattern */
class IPlayer : public XThread /* for synchronization */
{
public:
	static IPlayer* Get( unsigned char index=0 );
	virtual bool Open( const char* path );
	virtual void Close();
	virtual bool Start();
	virtual void InitView( void* win );
	bool isHardDecode = false;

	XParameter outPara; // Audio output parameters configuration

	IDemux* 	demux = 0;
	IDecode* 	vdecode = 0;
	IDecode* 	adecode = 0;
	IResample*	resample = 0;
	IVideoView* videoView = 0;
	IAudioPlay* audioPlay = 0;

protected:
	/* For the purpose of synchronizing */
	void Main();
	std::mutex mutex;
	IPlayer(){};
};

#endif