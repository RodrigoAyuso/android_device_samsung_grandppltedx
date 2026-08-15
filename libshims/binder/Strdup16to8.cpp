#include <utils/String8.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

extern "C" char* strndup16to8(const char16_t* s, size_t n) {
    if (s == nullptr) {
        return nullptr;
    }

    android::String8 utf8(s, n);
    return strdup(utf8.string());
}