#include "WwusersServer.h"
#include "WwusersDispatcher.h"
#include "WwusersThread.h"

void WwusersServer::init()
{
  userModel = new UserModel("/DATA/users.dat", "/DATA/user_terms.dat");
}

Thread *WwusersServer::createThreadObject()
{
        return (Thread *)(new WwusersThread());
}

Dispatcher *WwusersServer::createDispatcherObject()
{
        return (Dispatcher *)(new WwusersDispatcher());
}
