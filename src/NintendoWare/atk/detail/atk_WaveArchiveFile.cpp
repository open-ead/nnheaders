#include <nn/atk/atk_WaveArchiveFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const WaveArchiveFile::InfoBlock* WaveArchiveFile::FileHeader::GetInfoBlock() const { // 28
    return util::ConstBytePtr(this, GetInfoBlockOffset()).Get<InfoBlock>();
}

const WaveArchiveFile::FileBlock* WaveArchiveFile::FileHeader::GetFileBlock() const { // 34
    return util::ConstBytePtr(this, GetFileBlockOffset()).Get<FileBlock>();
}

u32 WaveArchiveFile::FileHeader::GetInfoBlockSize() const {
    return GetReferenceBy(ElementType_WaveArchiveFile_InfoBlock)->size;
}

u32 WaveArchiveFile::FileHeader::GetFileBlockSize() const {
    return GetReferenceBy(ElementType_WaveArchiveFile_FileBlock)->size;
}

u32 WaveArchiveFile::FileHeader::GetInfoBlockOffset() const {
    return GetReferenceBy(ElementType_WaveArchiveFile_InfoBlock)->offset;
}

u32 WaveArchiveFile::FileHeader::GetFileBlockOffset() const {
    return GetReferenceBy(ElementType_WaveArchiveFile_FileBlock)->offset;
}

const Util::ReferenceWithSize* WaveArchiveFile::FileHeader::GetReferenceBy(u16 typeId) const {
    for (int i {0}; i < BlockCount; ++i) {
        if (toBlocks[i].IsValidTypeId(typeId)) {
            return &toBlocks[i];
        }
    }

    return nullptr;
}
} // namespace nn::atk::detail