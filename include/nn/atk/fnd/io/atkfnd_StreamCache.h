#pragma once

#include <nn/atk/fnd/io/atkfnd_Stream.h>

namespace nn::atk::detail::fnd {
struct FsAccessLog;

class StreamCache {
public:
    constexpr static position_t InvalidPosition = -1;

    StreamCache();
    StreamCache(Stream* sourceStream, void* buffer, size_t length);

    void Initialize(Stream* sourceStream, void* buffer, size_t length);
    void Finalize();

    size_t Read(void* buf, size_t length, FndResult* result, FsAccessLog* log, void* pFileStream);
    
    void FlushWriteCache();
    
    size_t GetReadCacheHitLength(size_t) const;

    FndResult SyncStreamCurrentPosition(position_t position);

    void ClearCache();

    size_t Write(const void* buf, size_t length, FndResult* result);

    size_t GetWritableCacheLength(size_t) const;

    FndResult Seek(position_t offset, Stream::SeekOrigin origin);

    virtual ~StreamCache();

private:
    Stream* m_Stream{};
    position_t m_CurrentPosition{0};
    void* m_CacheBuffer{};
    size_t m_CacheBufferLength{0};
    position_t m_CachePosition{-1};
    size_t m_CachedLength{0};
    u8 m_CacheState{0};
    u8 m_Padding[3];
};
static_assert(sizeof(StreamCache) == 0x40);
} // namespace nn::atk::detail::fnd