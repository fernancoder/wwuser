#include "Remove_user_termAction.h"
#include "daework-support.h"

Remove_user_termAction *Remove_user_termAction::createHandler()
{
    return new Remove_user_termAction();
}

void Remove_user_termAction::execute()
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

