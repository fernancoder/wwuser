#ifndef WWUSERSSERVER_H_
#define WWUSERSSERVER_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "daework-support.h"
#include "UserModel.h"

class WwusersServer  : public Server
{
        public:
                UserModel *userModel;
                Dispatcher *dispatcherObject;

        public:
                WwusersServer(const char *cfgFilePath, const char *env, const char *module):Server(cfgFilePath, env, module){};

                void init();
                Thread *createThreadObject();
                Dispatcher *createDispatcherObject();

};
#endif /* WWUSERSSERVER_H_ */
