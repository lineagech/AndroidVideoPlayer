#ifndef XPLAY_FFPLAYERBUILDER_H
#define XPLAY_FFPLAYERBUILDER_H

class FFPlayerBuilder : public IPlayerBuilder
{
public:
	static void InitHard(void* vm);
	static FFPlayerBuilder* Get()
	{
		static FFPlayerBuilder ff_player_builder;
		return ff_player_builder;
	}

protected:
	FFPlayerBuilder(){};
	virtual IDemux* CreateDemux();
	virtual IDecode* CreateDecode();
	virtual IResample* CreateResample();
	virtual IVideoView* CreateVideoView();
	virtual IAudioPlay* CreateAudioPlay();
	virtual IPlayer* CreatePlayer( unsigned char index = 0 );
};

#endif