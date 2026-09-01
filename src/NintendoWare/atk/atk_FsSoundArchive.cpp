#include <nn/atk/atk_FsSoundArchive.h>

#include <nn/atk/fnd/os/atkfnd_ScopedLock.h>

namespace nn::atk {
FsSoundArchive::FsSoundArchive() = default;

FsSoundArchive::~FsSoundArchive() {
    Close();
}

void FsSoundArchive::Close() {
    FileAccessBegin();

    if (m_IsOpened) {
        m_FileStream.Close();
        m_IsOpened = false;
    }

    Finalize();
}

void FsSoundArchive::FileAccessBegin() const {
    if (m_FileAccessMode == FileAccessMode_InFunction) {
        auto lock = detail::fnd::ScopedLock<detail::fnd::CriticalSection>{m_FileOpenCloseLock};
        if (m_FileAccessCount == 0)
            m_FileStream.Open(m_SoundArchiveFullPath, detail::fnd::FileStream::AccessMode_Read);
    
        ++m_FileAccessCount;
    }
}
} // namespace nn::atk

