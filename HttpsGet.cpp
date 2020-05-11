//============================================================================
// Name        : SSLClient.cpp
// Compiling   : g++ -c -o SSLClient.o SSLClient.cpp
//               g++ -o SSLClient SSLClient.o -lssl -lcrypto
//============================================================================
#include "HttpsGet.h"

int HttpsGet::SendPacket(char *buf)
{
    int len = SSL_write(ssl, buf, strlen(buf));
    if (len < 0) {
        int err = SSL_get_error(ssl, len);

        printf("Error send:  %d\n", err);

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

int HttpsGet::RecvPacket()
{
    int len=100;
    char buf[1000000];
    do {
        printf("reading...");
        len=SSL_read(ssl, buf, 100);
        printf("readed...");
        buf[len]=0;
        printf("[%d]%s", len, buf);
        //printf("%d -----> %s\n", len, buf);
        //printf("%s", buf);
    } while (len > 100);
    do {
        printf("reading...");
        len=SSL_read(ssl, buf, 100);
        printf("readed...");
        buf[len]=0;
        printf("[%d]%s", len, buf);
        //printf("%d -----> %s\n", len, buf);
        //printf("%s", buf);
    } while (len > 100);


    printf("TERMINADO: %d\n", len);

    if (len < 0) {
        int err = SSL_get_error(ssl, len);
        printf("Error recive %d\n",err);
        if (err == SSL_ERROR_WANT_READ)
            return 0;
        if (err == SSL_ERROR_WANT_WRITE)
            return 0;
        if (err == SSL_ERROR_ZERO_RETURN || err == SSL_ERROR_SYSCALL || err == SSL_ERROR_SSL)
            return -1;
    }
}

void HttpsGet::log_ssl()
{
    int err;
    while (err = ERR_get_error()) {
        char *str = ERR_error_string(err, 0);
        if (!str)
            return;
        printf("%s",str);
        printf("\n");
        fflush(stdout);
    }
}

int HttpsGet::get(char *url)
{

    setbuf(stdout, NULL);

    printf("\n%s\n",url);

    int s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        printf("Error creating socket.\n");
        return -1;
    }
    struct sockaddr_in sa;
    memset (&sa, 0, sizeof(sa));
    sa.sin_family      = AF_INET;
    sa.sin_addr.s_addr = inet_addr("91.198.174.192"); //wikipedia.org
    sa.sin_port = htons(443);
    socklen_t socklen = sizeof(sa);
    if (connect(s, (struct sockaddr *)&sa, socklen)) {
        printf("Error connecting to server %d\n",errno);
        return -1;
    }
    SSL_library_init();
    SSLeay_add_ssl_algorithms();
    SSL_load_error_strings();
    const SSL_METHOD *meth = TLSv1_2_client_method();
    SSL_CTX *ctx = SSL_CTX_new (meth);
    ssl = SSL_new (ctx);
    if (!ssl) {
        printf("Error creating SSL.\n");
        log_ssl();
        return -1;
    }
    sock = SSL_get_fd(ssl);
    SSL_set_fd(ssl, s);
    int err = SSL_connect(ssl);
    if (err <= 0) {
        printf("Error creating SSL connection.  err=%x\n", err);
        log_ssl();
        fflush(stdout);
        return -1;
    }
    printf ("SSL connection using %s\n", SSL_get_cipher (ssl));

    /*char *request = "GET https://www.google.ru/intl/en/about/company/facts/ HTTP/1.1\r\n\r\n"*/
    SendPacket(url);
    RecvPacket();

    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return 0;
}