#ifndef ADD_USER_TERMACTION_H_
#define ADD_USER_TERMACTION_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "WwusersAction.h"

class Add_user_termAction : public WwusersAction
{
    public:
        static  Add_user_termAction *createHandler();
        void execute();
};

#endif /* ADD_USER_TERMACTION_H_ */
