#include "UserModel.h"

UserModel::UserModel(string user_file_path)
{
  this->user_file_path = user_file_path;
  pthread_mutex_init(&user_model_lock,NULL);

  //read file
  pthread_mutex_lock(&user_model_lock);


  pthread_mutex_unlock(&user_model_lock);
}

UserModel::~UserModel()
{
  pthread_mutex_destroy(&user_model_lock);
}


void UserModel::add_user(string user_id, string token)
{
  pthread_mutex_lock(&user_model_lock);


  pthread_mutex_unlock(&user_model_lock);
}


