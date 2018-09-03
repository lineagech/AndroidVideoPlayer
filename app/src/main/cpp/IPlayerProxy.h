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
	virtual bool Close();
	virtual bool Start();
	virtual void InitView( void* win );

protected:
	IPlayerProxy(){}; // Constructor
	IPlayer* player = 0;
	std::mutex mutex;
};

#endif