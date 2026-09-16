#include <nn/atk/atk_SoundArchiveLoader.h>

namespace nn::atk::detail {

SoundArchiveLoader::SoundArchiveLoader() = default;

SoundArchiveLoader::~SoundArchiveLoader() {
    m_pSoundArchive = nullptr;
};

void SoundArchiveLoader::SetSoundArchive(const SoundArchive* arc) {
    m_pSoundArchive = arc;
}

}  // namespace nn::atk::detail
