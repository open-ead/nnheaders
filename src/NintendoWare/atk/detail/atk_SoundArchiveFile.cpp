#include <nn/atk/atk_SoundArchiveFile.h>

namespace nn::atk::detail {
const Util::ReferenceWithSize* SoundArchiveFile::FileHeader::GetReferenceBy(u16 typeId) const {
    for (int i {0}; i < BlockCount; ++i) {
        if (toBlocks[i].typeId == typeId)
            return &toBlocks[i];
    }

    return nullptr;
}

u32 SoundArchiveFile::FileHeader::GetStringBlockSize() const {
    return GetReferenceBy(ElementType_SoundArchiveFile_StringBlock)->size;
}
} // namespace nn::atk::detail