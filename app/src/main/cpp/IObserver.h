//
// Created by Chia-Hao on 2018/8/26.
//

#ifndef XPLAY_IOBSERVER_H
#define XPLAY_IOBSERVER_H


#include "XData.h"
#include "XThread.h"
#include <vector>
#include <mutex>

/* Observer and main entity */
class IObserver: public XThread {
public:
    /* Observing function to receive data */
    virtual void Update(XData data) {}

    /* Main entity function, adding an observer */
    void AddObs( IObserver* observer );

    /* Notify all observers */
    void Notify(XData data);

protected:
    std::vector<IObserver*> observers;
    std::mutex mutex;
};


#endif //XPLAY_IOBSERVER_H
