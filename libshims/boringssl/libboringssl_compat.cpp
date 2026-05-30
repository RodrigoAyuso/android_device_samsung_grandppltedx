#include <stddef.h>

typedef struct ssl_st SSL;
typedef struct ssl_ctx_st SSL_CTX;

extern "C" {
    long SSL_ctrl(SSL *ssl, int cmd, long larg, void *parg) {
        return 1;
    }

    long SSL_CTX_ctrl(SSL_CTX *ctx, int cmd, long larg, void *parg) {
        return 1;
    }
}