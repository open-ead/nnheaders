#include <nn/atk/atk_FsSoundArchive.h>
#include "nn/atk/fnd/io/atkfnd_FileStream.h"
#include "nn/atk/fnd/os/atkfnd_ScopedLock.h"

namespace nn::atk {
FsSoundArchive::FsSoundArchive() = default;

void FsSoundArchive::Close() {
    FileAccessBegin();

    if (m_IsOpened) {
        m_FileStream.Close();
        m_IsOpened = false;
    }

    Finalize();
}
} // namespace nn::atk

