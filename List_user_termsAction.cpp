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


printf("Empiezo\n");

    if ( ((WwusersServer *)this->getServer())->userModel->user_exists(this->getRequestParam("user_uuid")) )
    {

printf("Hay cosas\n");

      string response = "";
      vector<UserTermRecord *> currentUserTermRecords = ((WwusersServer *)this->getServer())->userModel->list_user_terms(this->getRequestParam("user_uuid"));
      for( vector<UserTermRecord *>::iterator it = currentUserTermRecords.begin(); it != currentUserTermRecords.end(); ++it ) {

printf("%s\n",(*it)->entry_key);


        response += string("key: ") + (*it)->entry_key;

        delete *it;
      }
      this->sendSuccess(response);
    }
    else
        this->sendError(20001,"User not found");

    this->closeConnection();
}

