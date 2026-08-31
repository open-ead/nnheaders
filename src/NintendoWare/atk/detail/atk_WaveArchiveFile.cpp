#include <nn/atk/atk_WaveArchiveFile.h>

namespace nn::atk::detail {
const Util::ReferenceWithSize* WaveArchiveFile::FileHeader::GetReferenceBy(u16 typeId) const {
    for (int i {0}; i < BlockCount; ++i) {
        if (toBlocks[i].IsValidTypeId(typeId)) {
            return &toBlocks[i];
        }
    }

    return nullptr;
}
} // namespace nn::atk::detail