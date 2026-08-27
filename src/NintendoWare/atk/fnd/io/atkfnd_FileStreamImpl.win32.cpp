#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>

#include <nn/fs/fs_files.h>

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

void FileStreamImpl::Close() {
    if (IsOpened()) {
        m_CurrentPosition = 0;
        m_IsOpened = false;
        fs::CloseFile(m_FileHandle);
    }
}

void FileStreamImpl::Flush() {
    if (IsOpened())
        fs::FlushFile(m_FileHandle);
}

bool FileStreamImpl::IsOpened() const {
    return m_IsOpened;
}

// NON_MATCHING: needs symbol for nn::diag::detail::AbortImpl
size_t FileStreamImpl::GetSize() const {
    s64 fileSize;
    Result result {fs::GetFileSize(&fileSize, m_FileHandle)};

    if (result.IsSuccess())
        return fileSize & 0xffffffff;

    // diag::detail::AbortImpl("", "", "", 0);
}
} // namespace nn::atk::detail::fnd