#include "UserModel.h"
#include "HttpsGet.h"
#include "PushNotification.h"

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

//    printf("%s -> %s | %s -> %s", user_id.c_str(), (*it)->user_id, entry_key.c_str(), (*it)->entry_key );
    if ( user_id.compare((*it)->user_id) == 0 )
    {
      if ( entry_key.compare((*it)->entry_key) == 0 )
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

  time_t rawtime;
  struct tm * timeinfo;
  char buffer[80];
  time (&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer,sizeof(buffer),"%Y-%m-%dT%H:%M:%SZ",timeinfo);
  std::string str(buffer);
  strcpy(userTermRecord->last_update, str.c_str());

  userTermRecords.push_back(userTermRecord);
  push_user_terms();

  pthread_mutex_unlock(&user_model_lock);
}

vector<UserTermRecord *> UserModel::list_user_terms(string user_id)
{
  vector<UserTermRecord *> currentUserTermRecords;

  pthread_mutex_lock(&user_model_lock);
  for(vector<UserTermRecord *>::iterator it = userTermRecords.begin(); it != userTermRecords.end(); ++it) {
    if ( user_id.compare((*it)->user_id) == 0 ) {
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

void UserModel::remove_user_term(string user_id, string entry_key)
{
  pthread_mutex_lock(&user_model_lock);
  for(vector<UserTermRecord *>::iterator it = userTermRecords.begin(); it != userTermRecords.end(); ++it) {

//    printf("%s -> %s | %s -> %s", user_id.c_str(), (*it)->user_id, entry_key.c_str(), (*it)->entry_key );
    if ( user_id.compare((*it)->user_id) == 0 )
    {
      if ( entry_key.compare((*it)->entry_key) == 0 )
      {
        delete *it;
        userTermRecords.erase(it);
        push_user_terms();
        pthread_mutex_unlock(&user_model_lock);
        return;
      }
    }
  }
  pthread_mutex_unlock(&user_model_lock);
}

string UserModel::notify_changes()
{
  char update_date[21];
  string response;

  //TO-REMOVE
  setbuf(stdout, NULL);
  //TO-REMOVE

  pthread_mutex_lock(&user_model_lock);
  for(vector<UserTermRecord *>::iterator it = userTermRecords.begin(); it != userTermRecords.end(); ++it) {
    HttpsGet *httpsGet = new HttpsGet();
    if ( !httpsGet->getError() )
    {
      //string url = "GET https://es.wikipedia.org/w/api.php?action=query&prop=revisions&titles=" + string((*it)->entry_key) + "&rvlimit=1&rvslots=main&rvprop=timestamp%7Cuser%7Ccomment&rvdir=older&rvend=2050-01-01T00:00:00Z&format=json HTTP/1.1\r\nConnection: close\r\n\r\n";
      string url = "GET https://es.wikipedia.org/w/api.php?action=query&prop=revisions&titles=" + string((*it)->entry_key) + "&rvlimit=1&rvprop=timestamp%7Cuser%7Ccomment&format=json HTTP/1.1\r\nConnection: close\r\n\r\n";
      if ( httpsGet->get((char *)(url.c_str())) )
      {
        if ( httpsGet->stateOk() )
        {
          char *json_response = httpsGet->getJsonResponse();
          if ( json_response != NULL )
          {
            char *response_date = extract_update_date(json_response);
            if ( response_date != NULL )
            {
              memcpy(update_date, response_date, 20);
              update_date[20] = '\0';
              printf("[%s]%s -> %s (%s)\n", (*it)->user_id, (*it)->entry_title, update_date, (*it)->last_update);

              //if ( strcmp(update_date,(*it)->last_update) > 0 )
              if ( strcmp(update_date,(*it)->last_update) <= 0 )
              {
                //printf("Send change notification for %s to %s\n", (*it)->entry_title, (*it)->user_id);
                printf("Envío\n");
                send_notification((*it)->user_id, (*it)->entry_title);
              }
            }
          }
        }
      }
    }
    delete httpsGet;
  }
  pthread_mutex_unlock(&user_model_lock);

  return response;
}

void UserModel::send_notification(char *user_id, char *entry_title)
{ char payload[1024];
  for(vector<UserRecord *>::iterator it = userRecords.begin(); it != userRecords.end(); ++it) {
    if ( strcmp(user_id, (*it)->user_id) == 0 )
    {
      PushNotification *pushNotification = new PushNotification();
      if ( !pushNotification->getError() )
      {

        strcpy(payload, "{notification: {title: 'cambio',body: 'body', sound: 'default'},to: '");
        strcat(payload, (*it)->token);
        strcat(payload, "'}");

        string url = "POST https://fcm.googleapis.com/fcm/send HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: " + Util::intToString((int)strlen(payload)) + "\r\nAuthorization: key=AAAAdh166Bg:APA91bFLhhUmyIIrWakVXjZRyER3uOFgc_r6pJMvzTxWV7kW64aM3VovXGlrA1IKw2rdjxrNwyLP2IR64TLj9HuyOn5-Juj_YYzA7P1KqupAknfOFP8p28PjFezJNgFimmQYjEwNPoxz\r\nConnection: close\r\n\r\n";
        printf("ENVIO: %s\n%s", url.c_str(), payload);
        if ( pushNotification->get((char *)(url.c_str()), payload) )
        {


          printf("RESPUESTA: %s", pushNotification->getResponse());

          if ( pushNotification->stateOk() )
          {

          }
        }
      }
      delete pushNotification;
    }
  }
}

void UserModel::push_user_terms()
{
  FILE* USER_TERMS = fopen(user_term_file_path.c_str(), "wb");
  for(vector<UserTermRecord *>::iterator it = userTermRecords.begin(); it != userTermRecords.end(); ++it) {

    //printf("GRABO %s %s\n", (*it)->user_id, (*it)->entry_key);

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

char *UserModel::extract_update_date(char *json)
{
   char *cur_json = json;
    while ( true )
    {
      if ( *cur_json == '\0')
        return NULL;
      if ( memcmp(cur_json, "\"timestamp\":\"", 11 ) )
        cur_json++;
      else
        break;
    }

    return cur_json+13;
}
