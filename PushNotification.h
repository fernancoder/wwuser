#ifndef PUSH_NOTIFICATION_H_
#define PUSH_NOTIFICATION_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


//TO REMOVE
#include <unistd.h>
/////

//using namespace std;

class PushNotification
{
  private:
    SSL *ssl;
    SSL_CTX *ctx;
    int sock;
    char response[1000000];
    bool err_status;

  public:
    PushNotification();
    ~PushNotification();
    bool getError(){return err_status;};
    bool stateOk();
    char *getResponse(){return response;};
    bool get(char *url, char *entry_title);

  private:
   int RecvPacket();
   int SendPacket(char *buf, char *payload);
};


#endif /* PUSH_NOTIFICATION_H_ */