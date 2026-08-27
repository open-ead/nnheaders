#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>

namespace nn::atk::detail::fnd {
size_t FileStreamImpl::Read(void* buf, size_t length, FndResult* result) {
    if (IsCacheEnabled())
        return m_StreamCache.Read(buf, length, result, m_pAccessLog, this);

    return ReadDirect(buf, length, result);
}
} // namespace nn::atk::detail::fnd