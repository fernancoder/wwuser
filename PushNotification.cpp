//============================================================================
// Name        : SSLClient.cpp
// Compiling   : g++ -c -o SSLClient.o SSLClient.cpp
//               g++ -o SSLClient SSLClient.o -lssl -lcrypto
//============================================================================
#include "PushNotification.h"


PushNotification::PushNotification()
{
    err_status = false;
    int s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        printf("Error creating socket.\n");
        err_status = true;
        return;
    }
    struct sockaddr_in sa;
    memset (&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("74.125.193.95"); //googleapis.com
    sa.sin_port = htons(443);
    socklen_t socklen = sizeof(sa);
    if (connect(s, (struct sockaddr *)&sa, socklen)) {
        printf("Error connecting to server %d\n",errno);
        err_status = true;
        return;
    }
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *meth = TLSv1_2_client_method();
    ctx = SSL_CTX_new (meth);
    ssl = SSL_new (ctx);
    if (!ssl) {
        printf("Error creating SSL.\n");
        err_status = true;
        return;
    }
    sock = SSL_get_fd(ssl);
    SSL_set_fd(ssl, s);
    int err = SSL_connect(ssl);
    if (err <= 0) {
        printf("Error creating SSL connection.  err=%x\n", err);
        fflush(stdout);
        err_status = true;
        return;
    }
    //printf ("SSL connection using %s\n", SSL_get_cipher (ssl));
}

PushNotification::~PushNotification()
{
  SSL_free(ssl);
  SSL_CTX_free(ctx);
}

bool PushNotification::get(char *url, char *token, char *entry_title)
{
    //printf("URL:%s----->\n", url);
    if ( SendPacket(url, token, entry_title) )
    {
        RecvPacket();
        return true;
    }

    return false;
}

bool PushNotification::stateOk()
{
    return memcmp(response,"HTTP/1.1 200", 12) == 0;
}

/*char *PushNotification::getJsonResponse()
{
    char *cur_response = response;
    while ( true )
    {
      if ( *cur_response == '\0')
        return NULL;
      if ( memcmp(cur_response, "{\"continue\":{", 13) )
        cur_response++;
      else
        break;
    }

    return cur_response;
}*/

int PushNotification::SendPacket(char *buf, char *token, char *entry_title)
{
    char payload[1024];
    strcpy(payload, "{notification: {title: 'cambio',body: 'body', sound: 'default'},to: '");
    strcat(payload, token);
    strcat(payload, "'}");

    printf("%s\n", payload);

    int len = SSL_write(ssl, buf, strlen(buf));
    len = SSL_write(ssl, payload, strlen(payload));
    if (len < 0) {
        int err = SSL_get_error(ssl, len);
        switch (err) {
        case SSL_ERROR_WANT_WRITE:
            return 0;
        case SSL_ERROR_WANT_READ:
            return 0;
        case SSL_ERROR_ZERO_RETURN:
        case SSL_ERROR_SYSCALL:
        case SSL_ERROR_SSL:
        default:
            return -1;
        }
    }
}

int PushNotification::RecvPacket()
{
    int len=100;
    char buf[101];
    int cur_position = 0;
    response[0] = 0;

    do {
        len=SSL_read(ssl, buf, 100);
        buf[len]=0;
        memcpy(response + cur_position, buf, len + 1);
        cur_position += len;
    } while (len >= 100);
    do {
        len=SSL_read(ssl, buf, 100);
        buf[len]=0;
        memcpy(response + cur_position, buf, len + 1);
        cur_position += len;
    } while (len >= 100);
    if (len < 0) {
        int err = SSL_get_error(ssl, len);
        //printf("Error recive %d\n",err);
        if (err == SSL_ERROR_WANT_READ)
            return 0;
        if (err == SSL_ERROR_WANT_WRITE)
            return 0;
        if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)
            return -1;
    }

    return 0;
}