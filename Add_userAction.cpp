#include "Add_userAction.h"
#include "daework-support.h"
#include "WwusersServer.h"

Add_userAction *Add_userAction::createHandler()
{
    return new Add_userAction();
}

void Add_userAction::execute()
{

    if ( !verifyParam(this->getRequestParam("user_uuid"),1000,"Parameter user_uuid is mandatory") )
    {
	  this->closeConnection();
	  return;
    }
    if ( !verifyParam(this->getRequestParam("token"),1000,"Parameter token is mandatory") )
    {
	  this->closeConnection();
	  return;
    }

    ((WwusersServer *)this->getServer())->userModel->add_user(this->getRequestParam("user_uuid"), this->getRequestParam("token"));

    string response = "";
    this->sendSuccess(response);

    this->closeConnection();
}

