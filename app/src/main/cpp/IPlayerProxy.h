#ifndef XPLAY_IPLAYERPROXY_H
#define XPLAY_IPLAYERPROXY_H

#include "IPlayer.h"
#include <mutex>

class IPlayerProxy : public IPlayer 
{
public:
	static IPlayerProxy* Get()
	{
		static IPlayerProxy px;
		return &px;
	}
	void Init( void* vm = 0 );
	virtual bool Open( const char* path );
	virtual void Close();
	virtual bool Start();
	virtual void InitView( void* win );

    virtual double curr_playing_position();
	virtual void seek_update_progress( double progress );

protected:
	IPlayerProxy(){}; // Constructor
	IPlayer* player = 0;
	std::mutex mutex;
};

#endif