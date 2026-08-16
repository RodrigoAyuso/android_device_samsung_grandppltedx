#include <utils/String16.h>

#include <cstdlib>
#include <cstring>
#include <limits>

using android::String16;

extern "C" size_t strlen8to16(const char* utf8)
{
    if (utf8 == nullptr)
        return 0;

    String16 str(utf8);
    return str.size();
}

extern "C" char16_t* strcpy8to16(char16_t* dest,
                                 const char* utf8,
                                 size_t* out_len)
{
    if (dest == nullptr || utf8 == nullptr || out_len == nullptr)
        return nullptr;

    String16 str(utf8);
    const size_t len = str.size();

    if (len)
        memcpy(dest, str.string(), len * sizeof(char16_t));

    *out_len = len;
    return dest;
}

extern "C" char16_t* strcpylen8to16(char16_t* dest,
                                    const char* utf8,
                                    int length,
                                    size_t* out_len)
{
    if (dest == nullptr || utf8 == nullptr ||
        out_len == nullptr || length < 0)
        return nullptr;

    String16 str(utf8, static_cast<size_t>(length));
    const size_t len = str.size();

    if (len)
        memcpy(dest, str.string(), len * sizeof(char16_t));

    *out_len = len;
    return dest;
}

extern "C" char16_t* strdup8to16(const char* utf8,
                                 size_t* out_len)
{
    if (utf8 == nullptr || out_len == nullptr)
        return nullptr;

    String16 str(utf8);
    const size_t len = str.size();

    if (len > SIZE_MAX / sizeof(char16_t))
        return nullptr;

    /*
     * The legacy API does not require a trailing UTF-16 NUL.
     * Allocate one element for an empty string so the returned pointer
     * remains valid.
     */
    const size_t alloc_len = len ? len : 1;

    char16_t* result = static_cast<char16_t*>(
        malloc(alloc_len * sizeof(char16_t)));

    if (result == nullptr)
        return nullptr;

    if (len)
        memcpy(result, str.string(), len * sizeof(char16_t));

    *out_len = len;
    return result;
}