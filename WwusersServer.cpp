#include "WwusersServer.h"
#include "WwusersDispatcher.h"
#include "WwusersThread.h"

void WwusersServer::init()
{

}

Thread *WwusersServer::createThreadObject()
{
        return (Thread *)(new WwusersThread());
}

Dispatcher *WwusersServer::createDispatcherObject()
{
        return (Dispatcher *)(new WwusersDispatcher());
}
