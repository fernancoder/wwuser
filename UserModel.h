#ifndef USER_MODEL_H_
#define USER_MODEL_H_

#include "daework-include.h"
#include "daework-kernel.h"

class UserModel
{
    public:
      UserModel();
      ~UserModel():
      void add_user(char* user_id, char *token);
    private:
      pthread_mutex_t user_model_lock;

};

#endif /* USER_MODEL_H_ */

