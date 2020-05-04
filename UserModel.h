#ifndef USER_MODEL_H_
#define USER_MODEL_H_

#include "daework-include.h"
#include "daework-kernel.h"

struct UserRecord
{
   char user_id[256];
   char token[256];
};

struct UserTermRecord
{
   char user_id[256];
   char entry_key[256];
   char entry_title[256];
};

class UserModel
{
    public:
      UserModel(string user_file_path, string user_term_file_path);
      ~UserModel();
      void add_user(string user_id, string token);
      void add_user_term(string user_id, string entry_key, string entry_title);
      bool user_exists(string user_id);
    private:
      pthread_mutex_t user_model_lock;
      string user_file_path;
      string user_term_file_path;
      vector<UserRecord *> userRecords;
      vector<UserTermRecord *> userTermRecords;
      void push_users();
      void push_user_terms();
};

#endif /* USER_MODEL_H_ */

