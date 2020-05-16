#ifndef USER_MODEL_H_
#define USER_MODEL_H_

#include "daework-include.h"
#include "daework-kernel.h"
#include "daework-support.h"
#include <ctime>

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
   char last_update[20];
};

class UserModel
{
    public:
      UserModel(string user_file_path, string user_term_file_path);
      ~UserModel();
      void add_user(string user_id, string token);
      void add_user_term(string user_id, string entry_key, string entry_title);
      void remove_user_term(string user_id, string entry_key);
      bool user_exists(string user_id);
      bool user_term_exists(string user_id, string entry_key);
      string notify_changes();
      vector<UserTermRecord *> list_user_terms(string user_id);
    private:
      pthread_mutex_t user_model_lock;
      string user_file_path;
      string user_term_file_path;
      vector<UserRecord *> userRecords;
      vector<UserTermRecord *> userTermRecords;
      void push_users();
      void push_user_terms();
      char *extract_update_date(char *json);
      void send_notification(char *user_id, char *update_date, char *entry_title);
      string url_encode(const string &value);
};

#endif /* USER_MODEL_H_ */

