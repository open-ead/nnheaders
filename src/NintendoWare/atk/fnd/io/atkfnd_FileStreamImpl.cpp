#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>

namespace nn::atk::detail::fnd {
size_t FileStreamImpl::Read(void* buf, size_t length, FndResult* result) {
    if (IsCacheEnabled())
        return m_StreamCache.Read(buf, length, result, m_pAccessLog, this);

    return ReadDirect(buf, length, result);
}

size_t FileStreamImpl::Write(const void* buf, size_t length, FndResult* result) {
    if (IsCacheEnabled())
        return m_StreamCache.Write(buf, length, result);

    return WriteDirect(buf, length, result);
}

FndResult FileStreamImpl::Seek(position_t offset, SeekOrigin origin) {
    if (IsCacheEnabled())
        return m_StreamCache.Seek(offset, origin);

    return SeekDirect(offset, origin);
}

bool FileStreamImpl::CanRead() const {
    return IsOpened();
}

bool FileStreamImpl::CanWrite() const {
    return IsOpened();
}

bool FileStreamImpl::CanSeek() const {
    return IsOpened();
}
} // namespace nn::atk::detail::fnd