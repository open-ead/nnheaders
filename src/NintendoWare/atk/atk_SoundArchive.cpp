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
    m_FileBlockOffset = fileReader->GetFileBlockOffset();
}

void SoundArchive::Finalize() {
    if (m_pFileReader != nullptr) {
        m_pFileReader->Finalize();
        m_pFileReader = nullptr;
    }

    m_ExtFileRoot[0] = '/';
    m_ExtFileRoot[1] = '\0';
}

s32 SoundArchive::GetPlayerCount() const {
    return m_pFileReader->GetPlayerCount();
}

} // namespace nn::atk