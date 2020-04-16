#ifndef REMOVE_USER_TERMACTION_H_
#define REMOVE_USER_TERMACTION_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "WwusersAction.h"

class Remove_user_termAction : public WwusersAction
{
    public:
        static  Remove_user_termAction *createHandler();
        void execute();
};

#endif /* REMOVE_USER_TERMACTION_H_ */
