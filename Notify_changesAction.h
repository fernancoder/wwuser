#ifndef NOTIFY_CHANGES_H_
#define NOTIFY_CHANGES_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "WwusersAction.h"

class Notify_changesAction : public WwusersAction
{
    public:
        static  Notify_changesAction *createHandler();
        void execute();
};

#endif /* ADD_USERACTION_H_ */
