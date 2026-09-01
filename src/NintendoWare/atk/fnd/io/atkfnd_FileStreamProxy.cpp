#include <nn/atk/fnd/io/atkfnd_FileStreamProxy.h>

namespace nn::atk::detail::fnd {
FileStreamProxy::FileStreamProxy(FileStream* fileStream, position_t offset, size_t fileSize)
    : m_pFileStream{fileStream}, m_Offset{offset}, m_FileSize{fileSize} {}

FileStreamProxy::~FileStreamProxy() {
    m_Offset = 0;
    m_FileSize = 0;
    m_pFileStream = nullptr;
}
} // namespace nn::atk::detail::fnd