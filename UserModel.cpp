#include "UserModel.h"

UserModel::UserModel(string user_file_path, string user_term_file_path)
{
  this->user_file_path = user_file_path;
  this->user_term_file_path = user_term_file_path;
  pthread_mutex_init(&user_model_lock,NULL);
  pthread_mutex_lock(&user_model_lock);

  UserRecord *userRecord;
  FILE* USERS = fopen(user_file_path.c_str(), "rb");
  if ( USERS )
  {
    // File has been successfully opened, we can try to read data
    userRecord = new UserRecord();
    size_t recordsRead = fread(userRecord,sizeof(UserRecord),1,USERS);

    //printf("LEO PRIMERO %s %s\n", userRecord->user_id, userRecord->token);

    while ( recordsRead != 0 )
    {
      userRecords.push_back(userRecord);
      userRecord = new UserRecord();
      recordsRead = fread(userRecord, sizeof(UserRecord),1,USERS);

      //printf("LEO SIGUIENTES %s %s\n", userRecord->user_id, userRecord->token);
    }  // end of loop
    delete userRecord;
    fclose(USERS);
  }

  UserTermRecord *userTermRecord;
  FILE* USER_TERMS = fopen(user_term_file_path.c_str(), "rb");
  if ( USER_TERMS )
  {
    // File has been successfully opened, we can try to read data
    userTermRecord = new UserTermRecord();
    size_t recordsRead = fread(userTermRecord,sizeof(UserTermRecord),1,USER_TERMS);

    //printf("LEO PRIMERO %s %s\n", userTermRecord->user_id, userTermRecord->token);

    while ( recordsRead != 0 )
    {
      userTermRecords.push_back(userTermRecord);
      userTermRecord = new UserTermRecord();
      recordsRead = fread(userTermRecord, sizeof(UserTermRecord),1,USER_TERMS);

      //printf("LEO SIGUIENTES %s %s\n", userTermRecord->user_id, userTermRecord->token);
    }  // end of loop
    delete userTermRecord;
    fclose(USER_TERMS);
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
  for(vector<UserRecord *>::iterator it = userRecords.begin(); it != userRecords.end(); ++it) {

    //printf("GRABO %s %s\n", (*it)->user_id, (*it)->token);

    fwrite(*it,sizeof(UserRecord),1,USERS);
  }
  fclose(USERS);
}


void UserModel::add_user_term(string user_id, string entry_key, string entry_title)
{
  pthread_mutex_lock(&user_model_lock);
  for(vector<UserTermRecord *>::iterator it = userTermRecords.begin(); it != userTermRecords.end(); ++it) {
    if ( user_id.compare((*it)->user_id) != 0 )
    {
      if ( entry_key.compare((*it)->entry_key) != 0 )
      {
        pthread_mutex_unlock(&user_model_lock);
        return;
      }
    }
  }
  UserTermRecord *userTermRecord = new UserTermRecord();
  strcpy(userTermRecord->user_id, user_id.c_str());
  strcpy(userTermRecord->entry_key, entry_key.c_str());
  strcpy(userTermRecord->entry_title, entry_title.c_str());
  userTermRecords.push_back(userTermRecord);
  push_user_terms();

  pthread_mutex_unlock(&user_model_lock);
}

vector<UserTermRecord *> UserModel::list_user_terms(string user_id)
{
  vector<UserTermRecord *> currentUserTermRecords;

  pthread_mutex_lock(&user_model_lock);
  for(vector<UserTermRecord *>::iterator it = userTermRecords.begin(); it != userTermRecords.end(); ++it) {

printf("-------_> %s", (*it)->user_id);


    if ( user_id.compare((*it)->user_id) != 0 ) {
      UserTermRecord *userTermRecord = new UserTermRecord();
      strcpy(userTermRecord->user_id, (*it)->user_id);
      strcpy(userTermRecord->entry_key, (*it)->entry_key);
      strcpy(userTermRecord->entry_title, (*it)->entry_title);
      currentUserTermRecords.push_back(userTermRecord);
    }
  }
  pthread_mutex_unlock(&user_model_lock);

  return currentUserTermRecords;
}

void UserModel::push_user_terms()
{
  FILE* USER_TERMS = fopen(user_term_file_path.c_str(), "wb");
  for(vector<UserTermRecord *>::iterator it = userTermRecords.begin(); it != userTermRecords.end(); ++it) {

    //printf("GRABO %s %s\n", (*it)->user_id, (*it)->entry_key, (*it)->entry_title);

    fwrite(*it,sizeof(UserTermRecord),1,USER_TERMS);
  }
  fclose(USER_TERMS);
}

bool UserModel::user_exists(string user_id)
{
  pthread_mutex_lock(&user_model_lock);
  for(vector<UserRecord *>::iterator it = userRecords.begin(); it != userRecords.end(); ++it) {
    if ( user_id.compare((*it)->user_id) == 0 )
    {
      pthread_mutex_unlock(&user_model_lock);
      return true;
    }
  }

  pthread_mutex_unlock(&user_model_lock);
  return false;
}

bool UserModel::user_term_exists(string user_id, string entry_key)
{
  pthread_mutex_lock(&user_model_lock);
  for(vector<UserTermRecord *>::iterator it = userTermRecords.begin(); it != userTermRecords.end(); ++it) {
    if ( user_id.compare((*it)->user_id) == 0 )
    {
      if (  entry_key.compare((*it)->entry_key) == 0 )
      {
        pthread_mutex_unlock(&user_model_lock);
        return true;
      }
    }
  }

  pthread_mutex_unlock(&user_model_lock);
  return false;
}
