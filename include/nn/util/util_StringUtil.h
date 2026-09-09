#include <nn/types.h>

namespace nn::util {
template <typename T>
inline s32 Strlcpy(T* pOutDst, const T* pSrc, s32 count) {
    s32 length = 0;

    if (count > 0) {
        while (--count && *pSrc) {
            *pOutDst++ = *pSrc++;
            ++length;
        }
        *pOutDst++ = '\0';
    }

    while (*pSrc++)
        ++length;

    return length;
}

template <typename T>
inline s32 Strnlen(const T* pStr, s32 count) {
    s32 length = 0;

    if (count > 0) {
        while (count && *pStr) {
            ++pStr;
            ++length;
            --count;
        }
    }

    return length;
}

template <typename T>
inline s32 Strncmp(const T* pStr1, const T* pStr2, s32 count) {
    if (count == 0)
        return 0;

    T c1, c2;

    do {
        c1 = *pStr1++;
        c2 = *pStr2++;
    } while (c1 && c1 == c2 && --count);

    return c1 - c2;
}
}  // namespace nn::util