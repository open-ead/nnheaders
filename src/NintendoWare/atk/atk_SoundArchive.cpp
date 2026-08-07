#include <nn/atk/atk_SoundArchive.h>

#include <nn/atk/detail/atk_SoundArchiveFileReader.h>

namespace nn::atk {
SoundArchive::SoundArchive() {
    m_ExtFileRoot[0] = '/';
    m_ExtFileRoot[1] = '\0';
};

SoundArchive::~SoundArchive() = default;

bool SoundArchive::IsAvailable() const {
    return m_pFileReader != nullptr;
}

void SoundArchive::Initialize(detail::SoundArchiveFileReader* fileReader) {
    m_pFileReader = fileReader;
    m_FileBlockOffset = fileReader->m_Header.GetFileBlockOffset();
}
} // namespace nn::atk