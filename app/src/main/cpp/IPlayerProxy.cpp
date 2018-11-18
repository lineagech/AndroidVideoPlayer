#include "IPlayerProxy.h"
#include "FFPlayerBuilder.h"
#include "XLog.h"


void IPlayerProxy::Init( void* vm )
{
	mutex.lock();
	if( vm ) FFPlayerBuilder::InitHard(vm);
    if(!player)
        player = FFPlayerBuilder::Get()->BuilderPlayer();
	mutex.unlock();
}

void IPlayerProxy::Close()
{
	mutex.lock();
	if( player ) player->Close();
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
	mutex.lock();
	if( player )
        player->InitView( win );
	mutex.unlock();
}

void IPlayerProxy::Pause( bool pause_or_not )
{
    mutex.lock();
    if( player )
        player->Pause( pause_or_not );
    mutex.unlock();
}

bool IPlayerProxy::isPause()
{
    bool result = false;
    mutex.lock();
    if( player )
        result = player->isPause();
    mutex.unlock();
    return result;
}

double IPlayerProxy::curr_playing_position()
{
    double pos = 0.0;
    mutex.lock();
    if( player )
    {
        pos = player->curr_playing_position();
        //XLOGE("curr_playing_position %f", pos);
    }
    mutex.unlock();
    return pos;
}

void IPlayerProxy::seek_update_progress( double progress )
{
    mutex.lock();
    if( player )
    {
        player->seek_update_progress( progress );
    }
    mutex.unlock();
}









