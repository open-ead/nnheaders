#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>

namespace nn::atk::detail::fnd {
namespace {
fs::OpenMode ConvertAccessMode(FileStream::AccessMode accessMode) {

}

position_t GetSeekPosition(FileStreamImpl& target, position_t offset, Stream::SeekOrigin origin) {
    position_t targetSizeEnd;
    position_t result;
}
} // anonymous namespace

FileStreamImpl::FileStreamImpl() {
    m_DirectStream.m_Owner = this;
};
} // namespace nn::atk::detail::fnd