#ifndef USER_MODEL_H_
#define USER_MODEL_H_

#include "daework-include.h"
#include "daework-kernel.h"

class UserModel
{
    public:
      UserModel(string user_file_path);
      ~UserModel();
      void add_user(string user_id, string token);
    private:
      pthread_mutex_t user_model_lock;
      string user_file_path;

};

#endif /* USER_MODEL_H_ */

