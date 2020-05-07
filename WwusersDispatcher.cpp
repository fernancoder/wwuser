#include "WwusersDispatcher.h"
#include "Add_userAction.h"
#include "Add_user_termAction.h"
#include "Remove_user_termAction.h"
#include "List_user_termsAction.h"
#include "Notify_changesAction.h"



Action *WwusersDispatcher::getHandler(string action)
{
  if ( action.compare("add_user") == 0 )
    return Add_userAction::createHandler();

  if ( action.compare("add_user_term") == 0 )
    return Add_user_termAction::createHandler();

  if ( action.compare("remove_user_term") == 0 )
    return Remove_user_termAction::createHandler();

  if ( action.compare("list_user_terms") == 0 )
    return List_user_termsAction::createHandler();

  if ( action.compare("notify_changes") == 0 )
    return Notify_changesAction::createHandler();

  return defaultAction(action);
}
