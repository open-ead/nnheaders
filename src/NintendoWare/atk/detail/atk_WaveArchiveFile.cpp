#include <nn/atk/atk_WaveArchiveFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const Util::ReferenceWithSize* WaveArchiveFile::FileHeader::GetReferenceBy(u16 typeId) const {
    for (int i {0}; i < BlockCount; ++i) {
        if (toBlocks[i].IsValidTypeId(typeId)) {
            return &toBlocks[i];
        }
    }

    return nullptr;
}

const WaveArchiveFile::InfoBlock* WaveArchiveFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(this, GetInfoBlockOffset()).Get<InfoBlock>();
}

u32 WaveArchiveFile::FileHeader::GetInfoBlockOffset() const {
    return GetReferenceBy(ElementType_WaveArchiveFile_InfoBlock)->offset;
}

const WaveArchiveFile::FileBlock* WaveArchiveFile::FileHeader::GetFileBlock() const {
    return util::ConstBytePtr(this, GetFileBlockOffset()).Get<FileBlock>();
}

u32 WaveArchiveFile::FileHeader::GetFileBlockOffset() const {
    return GetReferenceBy(ElementType_WaveArchiveFile_FileBlock)->offset;
}

u32 WaveArchiveFile::FileHeader::GetInfoBlockSize() const {
    return GetReferenceBy(ElementType_WaveArchiveFile_InfoBlock)->size;
}
} // namespace nn::atk::detail