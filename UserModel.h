#ifndef USER_MODEL_H_
#define USER_MODEL_H_

#include "daework-include.h"
#include "daework-kernel.h"

struct UserRecord
{
   char user_id[256];
   char token[256];
};

class UserModel
{
    public:
      UserModel(string user_file_path);
      ~UserModel();
      void add_user(string user_id, string token);
    private:
      pthread_mutex_t user_model_lock;
      string user_file_path;
      vector<UserRecord *> userRecords;
      void push_users();
};

#endif /* USER_MODEL_H_ */

