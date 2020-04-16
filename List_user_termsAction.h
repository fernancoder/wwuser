#ifndef LIST_USER_TERMSACTION_H_
#define LIST_USER_TERMSACTION_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "WwusersAction.h"

class List_user_termsAction : public WwusersAction
{
    public:
        static  List_user_termsAction *createHandler();
        void execute();
};

#endif /* LIST_USER_TERMSACTION_H_ */
