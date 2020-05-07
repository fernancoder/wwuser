#include "Notify_changesAction.h"
#include "daework-support.h"
#include "WwusersServer.h"

Notify_changesAction *Notify_changesAction::createHandler()
{
    return new Notify_changesAction();
}

void Notify_changesAction::execute()
{
    this->sendSuccess("KAKILLA");
    //this->sendError(20001,"User not found");

    this->closeConnection();
}

