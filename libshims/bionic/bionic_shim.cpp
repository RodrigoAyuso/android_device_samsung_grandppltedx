#include <pthread.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {

  pid_t __pthread_gettid(pthread_t t) {
    return pthread_gettid_np(t);
  }

  void android_memset16(uint16_t* dst, uint16_t value, size_t size) {
    size >>= 1;

    while (size--) {
      *dst++ = value;
    }
  }

  void android_memset32(uint32_t* dst, uint32_t value, size_t size) {
    size >>= 2;

    while (size--) {
      *dst++ = value;
    }
  }

}