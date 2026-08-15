#include <openssl/ssl.h>

extern "C" const SSL_METHOD* SSLv3_client_method(void) {
    return TLS_client_method();
}