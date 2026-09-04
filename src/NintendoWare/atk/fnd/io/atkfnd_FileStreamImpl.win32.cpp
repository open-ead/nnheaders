#include <nn/atk/fnd/io/atkfnd_FileStreamImpl.h>

#include <algorithm>

#include <nn/fs/fs_files.h>

#include <nn/atk/fnd/basis/atkfnd_Inlines.h>

namespace nn::atk::detail::fnd {

namespace {
fs::OpenMode ConvertAccessMode(FileStream::AccessMode accessMode) {}

position_t GetSeekPosition(FileStreamImpl& target, position_t offset, Stream::SeekOrigin origin) {
    position_t targetSizeEnd = target.GetSize();
    position_t result{0};

    switch (origin) {
    case Stream::SeekOrigin_Begin:
        result = std::max(offset, 0L);
        break;
    case Stream::SeekOrigin_End:
        result = targetSizeEnd - std::min(offset, 0L);
        break;
    case Stream::SeekOrigin_Current:
        if (offset > 0)
            result = ClampMax(target.GetCurrentPosition() + offset, targetSizeEnd);

        else if (offset >= 0)
            result = target.GetCurrentPosition();

        else if (target.GetCurrentPosition() > -offset)
            result = target.GetCurrentPosition() + offset;

        break;
    default:
        result = 0;
        break;
    }

    return result;
}
}  // anonymous namespace

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
    Result result{fs::GetFileSize(&fileSize, m_FileHandle)};

    if (result.IsSuccess())
        return fileSize & 0xffffffff;

    // diag::detail::AbortImpl("", "", "", 0);
}

int FileStreamImpl::GetIoBufferAlignment() const {
    return 1;
}

size_t FileStreamImpl::ReadDirect(void* buf, size_t length, FndResult* result) {
    ValidateAlignment(buf);

    size_t readFileLength{0};
    Result nnResult{fs::ReadFile(&readFileLength, m_FileHandle, m_CurrentPosition, buf, length)};

    FndResult readResult;

    if (nnResult.IsSuccess()) {
        readResult = FndResult{readFileLength != length};
        m_CurrentPosition += readFileLength;
    } else {
        readResult = FndResult{FndResultType_IoError};
    }

    if (result != nullptr)
        *result = readResult;

    return readFileLength;
}

size_t FileStreamImpl::WriteDirect(const void* buf, size_t length, FndResult* result) {
    ValidateAlignment(buf);

    FndResult writeResult;

    fs::WriteOption option;
    Result nnResult{fs::WriteFile(m_FileHandle, m_CurrentPosition, buf, length, option)};

    if (nnResult.IsSuccess()) {
        writeResult = FndResult{FndResultType_True};
        m_CurrentPosition += length;
    } else {
        writeResult = FndResult{FndResultType_IoError};
    }

    if (result != nullptr)
        *result = writeResult;

    return length;
}

FndResult FileStreamImpl::SeekDirect(position_t offset, SeekOrigin origin) {
    position_t seekPosition{GetSeekPosition(*this, offset, origin)};
    m_CurrentPosition = seekPosition;
    return FndResult{FndResultType_True};
}

}  // namespace nn::atk::detail::fnd