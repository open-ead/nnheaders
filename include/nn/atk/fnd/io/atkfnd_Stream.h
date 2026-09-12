#pragma once

#include <nn/types.h>
#include <nn/util.h>

#include <nn/atk/fnd/basis/atkfnd_Config.h>
#include <nn/atk/fnd/basis/atkfnd_Result.h>

namespace nn::atk::detail::fnd {
class Stream {
    NN_NO_COPY(Stream);
public:
    static const position_t InvalidPosition = 0xffffffff;
    static const size_t InvalidSize = 0xffffffff;

    enum SeekOrigin {
        SeekOrigin_Begin,
        SeekOrigin_End,
        SeekOrigin_Current,
    };

    virtual ~Stream() = default;

protected:
    Stream() = default;

public:
    virtual void Close() = 0;
    virtual bool IsOpened() const = 0;
    
    virtual size_t Read(void* buf, size_t length, FndResult* result);
    virtual size_t Write(const void* buf, size_t length, FndResult* result);
    virtual FndResult Seek(position_t offset, SeekOrigin origin);
    
    virtual position_t GetCurrentPosition() const;
    virtual size_t GetSize() const;
    
    virtual bool CanRead() const;
    virtual bool CanWrite() const;
    virtual bool CanSeek() const;
};
static_assert(sizeof(Stream) == 0x8);
} // namespace nn::atk::detail::fnd