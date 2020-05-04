#include "Add_user_termAction.h"
#include "daework-support.h"
#include "WwusersServer.h"

Add_user_termAction *Add_user_termAction::createHandler()
{
    return new Add_user_termAction();
}

void Add_user_termAction::execute()
{

    if ( !verifyParam(this->getRequestParam("user_uuid"),1000,"Parameter user_uuid is mandatory") )
    {
	this->closeConnection();
	return;
    }
    if ( !verifyParam(this->getRequestParam("term"),1000,"Parameter term is mandatory") )
    {
	this->closeConnection();
	return;
    }

    if ( ((WwusersServer *)this->getServer())->userModel->user_exists(this->getRequestParam("user_uuid")) )
    {
        //Change this and put your stuff here
        string response = "\"stuff\" : \"OK\"";
        this->sendSuccess(response);
        //end
    }
    else
        this->sendError(20001,"User not found");

    this->closeConnection();
}

