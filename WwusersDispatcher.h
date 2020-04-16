#ifndef WWUSERSDISPATCHER_H_
#define WWUSERSDISPATCHER_H_

#include "daework-kernel.h"


class WwusersDispatcher : public Dispatcher
{
        public:
                Action *getHandler(string action);

};

#endif /* WWUSERSDISPATCHER_H_ */
