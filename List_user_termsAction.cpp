#include "List_user_termsAction.h"
#include "daework-support.h"

List_user_termsAction *List_user_termsAction::createHandler()
{
    return new List_user_termsAction();
}

void List_user_termsAction::execute()
{

    if ( !verifyParam(this->getRequestParam("user_uid"),1000,"Parameter user_uid is mandatory") )
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

