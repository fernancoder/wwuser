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
    if ( !verifyParam(this->getRequestParam("entry_key"),1000,"Parameter entry_key is mandatory") )
    {
	   this->closeConnection();
	   return;
    }
    if ( !verifyParam(this->getRequestParam("entry_title"),1000,"Parameter entry_title is mandatory") )
    {
        this->closeConnection();
        return;
    }

    if ( ((WwusersServer *)this->getServer())->userModel->user_exists(this->getRequestParam("user_uuid")) )
    {
        if ( ((WwusersServer *)this->getServer())->userModel->user_term_exists(this->getRequestParam("user_uuid"), this->getRequestParam("entry_key")) )
            this->sendError(20002,"User is allready suscribed to this entry");
        else
        {
            ((WwusersServer *)this->getServer())->userModel->add_user_term(
                this->getRequestParam("user_uuid"),
                this->getRequestParam("entry_key"),
                this->getRequestParam("entry_title")
            );
            string response = "";
            this->sendSuccess(response);
        }
    }
    else
        this->sendError(20001,"User not found");

    this->closeConnection();
}

