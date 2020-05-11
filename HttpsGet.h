#ifndef HTTPS_GET_TERMSACTION_H_
#define HTTPS_GET_TERMSACTION_H_

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

class HttpsGet
{
  private:
    SSL *ssl;
    int sock;

  public:
    int get(char *url);

  private:
   int RecvPacket();
   int SendPacket(char *buf);
   void log_ssl();
};


#endif /* HTTPS_GET_TERMSACTION_H_ */