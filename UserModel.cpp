#include "UserModel.h"

UserModel::UserModel(string user_file_path)
{
  this->user_file_path = user_file_path;
  pthread_mutex_init(&user_model_lock,NULL);

  UserRecord *userRecord;

  pthread_mutex_lock(&user_model_lock);
  FILE* USERS = fopen(user_file_path.c_str(), "rb");
  if ( USERS )
  {
    // File has been successfully opened, we can try to read data
    userRecord = new UserRecord();
    recordsRead = fread(&userRecord,sizeof(UserRecord),1,USERS);
    while ( recordsRead != 0 )
    {
      userRecords.push_back(userRecord);
      userRecord = new UserRecord();
      recordsRead = fread(&userRecord, sizeof(UserRecord),1,USERS);
    }  // end of loop
    delete userRecord;
    fclose(USERS);
  }

  pthread_mutex_unlock(&user_model_lock);
}

UserModel::~UserModel()
{
  pthread_mutex_destroy(&user_model_lock);
}


void UserModel::add_user(string user_id, string token)
{
  pthread_mutex_lock(&user_model_lock);

  for(vector<UserRecord *>::iterator it = v.begin(); it != v.end(); ++it) {
    if ( it->user_id.compare(user_id) == 0 )
    {
      if ( it->token.compare(token) != 0 )
      {
        it->token = token
        push_users();
      }
      pthread_mutex_unlock(&user_model_lock);
      return;
    }
    userRecord *UserRecord = new UserRecord();
    strcpy(userRecord->user_id, user_id.c_str();
    strcpy(userRecord->token, token.c_str();
    userRecords.push_back(userRecord);
    push_users();
  }

  pthread_mutex_unlock(&user_model_lock);
}

void UserModel::push_users()
{
  FILE* USERS = fopen(user_file_path.c_str(), "wb");
  for(vector<UserRecord *>::iterator it = v.begin(); it != v.end(); ++it) {
     fwrite(it,sizeof(UserRecord),1,USERS);
  }
}



