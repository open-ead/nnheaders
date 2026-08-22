namespace nn::util {
template <typename T>
inline int Strlcpy(T* pOutDst, const T* pSrc, int count) {
    int length = 0;

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
inline int Strnlen(const T* pStr, int count) {
    int length = 0;
    
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
inline int Strncmp(const T* pStr1, const T* pStr2, int count) {
    if (count == 0)
        return 0;

    T c1, c2;
    
    do {    
        c1 = *pStr1++;
        c2 = *pStr2++;
    } while (c1 && c1 == c2 && --count);

    return c1 - c2;
}
} // namespace nn::util