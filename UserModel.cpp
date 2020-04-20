#include "UserModel.h"

UserModel::UserModel()
{
  pthread_mutex_init(&user_model_lock,NULL);
}

UserModel::~UserModel()
{
  pthread_mutex_destroy(&user_model_lock);
}


void UserModel::add_user(char* user_id, char *token)
{
   pthread_mutex_lock(&user_model_lock);


   pthread_mutex_unlock(&user_model_lock);
}


