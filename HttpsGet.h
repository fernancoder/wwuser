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
    SSL_CTX *ctx;
    int sock;
    char response[1000000];
    bool err_status;

  public:
    HttpsGet();
    ~HttpsGet();
    bool getError(){return err_status;};
    char *getResponse(){return response;};
    bool get(char *url);

  private:
   int RecvPacket();
   int SendPacket(char *buf);
};


#endif /* HTTPS_GET_TERMSACTION_H_ */