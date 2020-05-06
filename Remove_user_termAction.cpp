#include "Remove_user_termAction.h"
#include "daework-support.h"
#include "WwusersServer.h"

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

    if ( ((WwusersServer *)this->getServer())->userModel->user_exists(this->getRequestParam("user_uuid")) )
    {
        if ( !((WwusersServer *)this->getServer())->userModel->user_term_exists(this->getRequestParam("user_uuid"), this->getRequestParam("entry_key")) )
            this->sendError(20002,"User is not suscribed to this entry");
        else
        {
            ((WwusersServer *)this->getServer())->userModel->remove_user_term(
                this->getRequestParam("user_uuid"),
                this->getRequestParam("entry_key")
            );
            string response = "";
            this->sendSuccess(response);
        }
    }
    else
        this->sendError(20001,"User not found");

    this->closeConnection();
}

