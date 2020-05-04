#include "List_user_termsAction.h"
#include "daework-support.h"
#include "WwusersServer.h"

List_user_termsAction *List_user_termsAction::createHandler()
{
    return new List_user_termsAction();
}

void List_user_termsAction::execute()
{

    if ( !verifyParam(this->getRequestParam("user_uuid"),1000,"Parameter user_uuid is mandatory") )
    {
	   this->closeConnection();
	   return;
    }

    if ( ((WwusersServer *)this->getServer())->userModel->user_exists(this->getRequestParam("user_uuid")) )
    {
      vector<UserTermRecord *> currentUserTermRecords = ((WwusersServer *)this->getServer())->userModel->list_user_terms(this->getRequestParam("user_uuid"));
      //string response = "\"totalResults\":" + currentUserTermRecords.count() + ", [";
      string response = string("\"totalResults\":") + string("2") + string(", \"records\": [");
      bool is_first = true;
      for( vector<UserTermRecord *>::iterator it = currentUserTermRecords.begin(); it != currentUserTermRecords.end(); ++it ) {
        if ( !is_first )
          response += string(",");

        response += ( string("{ \"key\": \"") + string((*it)->entry_key) + string("\", \"title\": \"") + string((*it)->entry_title) + string("\"}") );

        is_first = false;
        delete *it;
      }

      response += string("]");
      this->sendSuccess(response);
    }
    else
        this->sendError(20001,"User not found");

    this->closeConnection();
}

