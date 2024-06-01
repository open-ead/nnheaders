#pragma once

namespace nn {
namespace util {

template <typename T>
int Strlcpy(T* pOutDst, const T* pSrc, int count) {
    int length = 0;

    while (--count && *pSrc) {
        *pOutDst++ = *pSrc++;
        ++length;
    }
    *pOutDst = '\0';

    while (*pSrc++) {
        ++length;
    }

    return length;
}

}  // namespace util
}  // namespace nn
