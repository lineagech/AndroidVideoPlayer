
#ifndef XPLAY_IPLAYER_H
#define XPLAY_IPLAYER_H

class IDemux; /* declare first so that do not need to include header file */
class IDecode;
class IResample;
class IVideoView;
class IAudioPlay;

/* Fascade Pattern */
class IPlayer : public XThread /* for synchronization */
{
public:
	static IPlayer* Get( unsigned char index=0 );
	virtual static void InitHard(void* vm);
	virtual bool Open( const char* patch );
	virtual bool Start();
	virtual void InitView( void* win );
	bool isHardDecode = false;

	IDemux* 	demux = 0;
	IDecode* 	vdecode = 0;
	IDecode* 	adecode = 0;
	IResample*	resample = 0;
	IVideoView* videoView = 0;
	IAudioPlay* audioPlay = 0;

	XParameter outPara; // Audio output parameters configuration

protected:
	IPlayer(){};
};

#endif