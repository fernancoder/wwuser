#include "List_user_eventsAction.h"
#include "daework-support.h"
#include "WwusersServer.h"

List_user_eventsAction *List_user_eventsAction::createHandler()
{
    return new List_user_eventsAction();
}

void List_user_eventsAction::execute()
{

    if ( !verifyParam(this->getRequestParam("user_uuid"),1000,"Parameter user_uuid is mandatory") )
    {
	    this->closeConnection();
	    return;
    }

    if ( ((WwusersServer *)this->getServer())->userModel->user_exists(this->getRequestParam("user_uuid")) )
    {
      vector<UserEventRecord *> currentUserEventRecord = ((WwusersServer *)this->getServer())->userModel->list_user_events(this->getRequestParam("user_uuid"));
      string response = string("\"totalResults\":") + Util::intToString((int)(currentUserEventRecord.size())) + string(", \"records\": [");
      bool is_first = true;

      for( vector<UserEventRecord *>::iterator it = currentUserEventRecord.begin(); it != currentUserEventRecord.end(); ++it ) {
        if ( !is_first )
          response += string(",");

        response += ( string("{ \"creation_date\": \"") + string((*it)->creation_date) + string("\", \"title\": \"") + string((*it)->entry_title) + string("\"}") );

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

