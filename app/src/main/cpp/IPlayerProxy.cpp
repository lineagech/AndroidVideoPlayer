#include "IPlayerProxy.h"


void IPlayerProxy::Init( void* vm )
{
	mutex.lock();
	if( vm ) FFPlayerBuilder::InitHard(vm);
	if( !player ) player = FFPlayerBuilder::Get()->BuilderPlayer();
	mutex.unlock();
}

bool IPlayerProxy::Open( const char* path )
{	
	bool re = false;
	mutex.lock();
	if( player ) re = player->Open(path);
	mutex.unlock();

	return re;
}

bool IPlayerProxy::Start()
{	
	bool re = false;
	mutex.lock();
	if( player ) re = player->Start();
	mutex.unlock();

	return re;
}

void IPlayerProxy::InitView( void* win )
{	
	bool re = false;
	mutex.lock();
	if( player ) re = player->InitView(win);
	mutex.unlock();

	return re;
}









