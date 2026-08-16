#include <openssl/ssl.h>

extern "C" const SSL_METHOD* SSLv3_client_method(void) {
    return TLS_client_method();
}

extern "C" const SSL_METHOD* SSLv3_server_method(void) {
    return TLS_server_method();
}