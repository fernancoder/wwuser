#include "Notify_changesAction.h"
#include "daework-support.h"
#include "WwusersServer.h"

Notify_changesAction *Notify_changesAction::createHandler()
{
    return new Notify_changesAction();
}

void Notify_changesAction::execute()
{
    string response = ((WwusersServer *)this->getServer())->userModel->notify_changes();
    this->sendSuccess(response);

    this->closeConnection();
}

