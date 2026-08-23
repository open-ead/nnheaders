#pragma once

#include <cstring>

#include <nn/atk/fnd/basis/atkfnd_Config.h>

namespace nn::atk::detail::fnd {
// UNSURE
inline size_t strncat(char* dst, size_t dstCount, const char* src, size_t srcCount) {
    size_t length {strlen(src)};
    if (length + srcCount + 1 < dstCount)
        length = srcCount + 1;
    else
        length = dstCount - 1 - length;

    std::strncat(dst, src, length);
    return length;
}
} // namespace atk::detail::fnd