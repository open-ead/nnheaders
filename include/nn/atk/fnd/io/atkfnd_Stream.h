#pragma once

#include <nn/types.h>

#include <nn/atk/fnd/basis/atkfnd_Config.h>
#include <nn/atk/fnd/basis/atkfnd_Result.h>

namespace nn::atk::detail::fnd {
class Stream {
public:
    enum SeekOrigin {
        SeekOrigin_Begin,
        SeekOrigin_End,
        SeekOrigin_Current,
    };

    virtual ~Stream();
    
    virtual void Close() = 0;
    virtual bool IsOpened() const = 0;
    
    virtual std::size_t Read(void* buf, std::size_t length, FndResult* result) = 0;
    virtual std::size_t Write(const void* buf, std::size_t length, FndResult* result) = 0;
    virtual FndResult Seek(position_t offset, SeekOrigin origin) = 0;
    
    virtual position_t GetCurrentPosition() const = 0;
    virtual std::size_t GetSize() const = 0;
    
    virtual bool CanRead() const = 0;
    virtual bool CanWrite() const = 0;
    virtual bool CanSeek() const = 0;
};
static_assert(sizeof(Stream) == 0x8);
} // namespace nn::atk::detail::fnd