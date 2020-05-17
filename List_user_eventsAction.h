#ifndef LIST_USER_EVENTSACTION_H_
#define LIST_USER_EVENTSACTION_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "WwusersAction.h"

class List_user_eventsAction : public WwusersAction
{
    public:
        static  List_user_eventsAction *createHandler();
        void execute();
};

#endif /* LIST_USER_EVENTSACTION_H_ */
