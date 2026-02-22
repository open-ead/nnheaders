#pragma once

#include <nn/atk/fnd/basis/atkfnd_Config.h>
#include <nn/atk/fnd/basis/atkfnd_Result.h>
#include <nn/atk/fnd/io/atkfnd_Stream.h>

namespace nn::atk::detail::fnd {
struct FsAccessLog;

class StreamCache {
public:
    StreamCache();
    StreamCache(Stream* sourceStream, void* buffer, std::size_t length);

    void Initialize(Stream* sourceStream, void* buffer, std::size_t length);
    void Finalize();

    std::size_t Read(void* buf, std::size_t length, FndResult* result, FsAccessLog* log, void* pFileStream);
    
    void FlushWriteCache();
    
    std::size_t GetReadCacheHitLength(std::size_t) const;

    FndResult SyncStreamCurrentPosition(position_t position);

    void ClearCache();

    std::size_t Write(void* buf, std::size_t length, FndResult* result);

    std::size_t GetWritableCacheLength(std::size_t) const;

    FndResult Seek(position_t offset, Stream::SeekOrigin origin);

    virtual ~StreamCache();

private:
    Stream* m_Stream{};
    position_t m_CurrentPosition{0};
    void* m_CacheBuffer{};
    std::size_t m_CacheBufferLength{0};
    position_t m_CachePosition{-1};
    std::size_t m_CachedLength{0};
    u8 m_CacheState{0};
    u8 m_Padding[3];
};
static_assert(sizeof(StreamCache) == 0x40);
} // namespace nn::atk::detail::fnd