#ifndef ADD_USERACTION_H_
#define ADD_USERACTION_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "WwusersAction.h"

class Add_userAction : public WwusersAction
{
    public:
        static  Add_userAction *createHandler();
        void execute();
};

#endif /* ADD_USERACTION_H_ */
