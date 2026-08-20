#include <nn/atk/detail/atk_AddonSoundArchiveContainer.h>

#include <nn/util/util_StringUtil.h>

namespace nn::atk::detail {
AddonSoundArchiveContainer::AddonSoundArchiveContainer() = default;

AddonSoundArchiveContainer::~AddonSoundArchiveContainer() {
    m_IsActive = false;
    m_pSoundArchive = nullptr;
    m_pSoundDataManager = nullptr;
}

bool AddonSoundArchiveContainer::Initialize(const char* soundArchiveName, 
                                            const AddonSoundArchive* pSoundArchive, 
                                            const SoundDataManager* pSoundDataManager) {
    util::Strlcpy(m_SoundArchiveName, soundArchiveName, sizeof(m_SoundArchiveName));
    m_IsActive = true;
    m_pSoundArchive = pSoundArchive;
    m_pSoundDataManager = pSoundDataManager;

    return true;
}

void AddonSoundArchiveContainer::Finalize() {
    m_IsActive = false;
    m_SoundArchiveName[0] = '\0';
    m_pSoundArchive = nullptr;
    m_pSoundDataManager = nullptr;
}
} // namespace nn::atk::detail