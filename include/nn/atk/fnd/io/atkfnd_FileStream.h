#pragma once

#include <nn/atk/fnd/io/atkfnd_Stream.h>

namespace nn::atk::detail::fnd {
class FileStream : Stream {
public:
    enum AccessMode {
        AccessMode_None,
        AccessMode_Read,
        AccessMode_Write,
        AccessMode_ReadAndWrite,
        AccessMode_AllowAppend,
        AccessMode_AllowAppendAndWrite = 6,
    };

    ~FileStream() override;
};
static_assert(sizeof(FileStream) == 0x8);
} // namespace nn::atk::detail::fnd