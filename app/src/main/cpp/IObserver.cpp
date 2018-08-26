//
// Created by Chia-Hao on 2018/8/26.
//

#include "IObserver.h"

/* Main entity function, adding an observer */
void IObserver::AddObs( IObserver* observer )
{
    if( observer == nullptr ) return;
    mutex.lock();
    observers.push_back( observer );
    mutex.unlock();
}

/* Notify all observers */
void IObserver::Notify(XData data)
{
    mutex.lock();
    for( int i = 0; i < observers.size(); i++ )
    {
        observers[i]->Update(data);
    }
    mutex.unlock();
}