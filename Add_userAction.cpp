#include "Add_userAction.h"
#include "daework-support.h"

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


    //Change this and put your stuff here
    string response = "\"stuff\" : \"OK\"";
    this->sendSuccess(response);
    //end

    this->closeConnection();
}

