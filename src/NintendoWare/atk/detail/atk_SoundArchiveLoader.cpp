#include <nn/atk/atk_SoundArchiveLoader.h>

namespace nn::atk::detail {

SoundArchiveLoader::SoundArchiveLoader() = default;

SoundArchiveLoader::~SoundArchiveLoader() {
    m_pSoundArchive = nullptr;
};

}  // namespace nn::atk::detail
