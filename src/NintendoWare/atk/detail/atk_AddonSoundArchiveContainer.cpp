#include <nn/atk/detail/atk_AddonSoundArchiveContainer.h>

namespace nn::atk::detail {
AddonSoundArchiveContainer::AddonSoundArchiveContainer() = default;

void AddonSoundArchiveContainer::Finalize() {
    m_IsActive = false;
    m_SoundArchiveName[0] = '\0';
    m_pSoundArchive = nullptr;
    m_pSoundDataManager = nullptr;
}
} // namespace nn::atk::detail