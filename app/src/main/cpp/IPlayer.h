
#ifndef XPLAY_IPLAYER_H
#define XPLAY_IPLAYER_H

class IDemux; /* declare first so that do not need to include header file */
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;

class IPlayer : public XThread /* for synchronization */
{
public:
	static IPlayer* Get( unsigned char index=0 );
	virtual bool Open( const char* patch );
	virtual bool Start();
	bool isHardDecode = false;

	IDemux* 	demux = 0;
	IDecode* 	vdecode = 0;
	IDecode* 	adecode = 0;
	IResample*	resample = 0;
	IVideoView* videoView = 0;
	IAudioPlay* audioPlay = 0;

protected:
	IPlayer(){};
};

#endif