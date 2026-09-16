#include <nn/atk/atk_SoundArchiveLoader.h>

namespace nn::atk::detail {

SoundArchiveLoader::SoundArchiveLoader() = default;

SoundArchiveLoader::~SoundArchiveLoader() {
    m_pSoundArchive = nullptr;
};

void SoundArchiveLoader::SetSoundArchive(const SoundArchive* arc) {
    m_pSoundArchive = arc;
}

bool SoundArchiveLoader::IsAvailable() const {
    if (m_pSoundArchive == nullptr)
        return false;

    return m_pSoundArchive->IsAvailable();
}

}  // namespace nn::atk::detail
