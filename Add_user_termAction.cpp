#include "Add_user_termAction.h"
#include "daework-support.h"

Add_user_termAction *Add_user_termAction::createHandler()
{
    return new Add_user_termAction();
}

void Add_user_termAction::execute()
{

    if ( !verifyParam(this->getRequestParam("user_uid"),1000,"Parameter user_uid is mandatory") )
    {
	this->closeConnection();
	return;
    }
    if ( !verifyParam(this->getRequestParam("term"),1000,"Parameter term is mandatory") )
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

