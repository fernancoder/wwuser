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
    size_t recordsRead = fread(&userRecord,sizeof(UserRecord),1,USERS);
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


  printf("ANYADO\n");

  for(vector<UserRecord *>::iterator it = userRecords.begin(); it != userRecords.end(); ++it) {
    if ( user_id.compare((*it)->user_id) == 0 )
    {
      if ( token.compare((*it)->token) != 0 )
      {
        strcpy((*it)->token, token.c_str());
        push_users();
      }
      pthread_mutex_unlock(&user_model_lock);
      return;
    }
  }
  UserRecord *userRecord = new UserRecord();
  strcpy(userRecord->user_id, user_id.c_str());
  strcpy(userRecord->token, token.c_str());
  userRecords.push_back(userRecord);
  push_users();

  pthread_mutex_unlock(&user_model_lock);
}

void UserModel::push_users()
{
  FILE* USERS = fopen(user_file_path.c_str(), "wb");

    printf("ENTRO\n");

  for(vector<UserRecord *>::iterator it = userRecords.begin(); it != userRecords.end(); ++it) {

    printf("HOLA\n");

     fwrite(it,sizeof(UserRecord),1,USERS);
  }
}



