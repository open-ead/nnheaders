#include <nn/atk/atk_StreamSoundFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
bool StreamSoundFile::FileHeader::HasSeekBlock() const {
    return GetReferenceBy(ElementType_StreamSoundFile_SeekBlock) != nullptr;
}

bool StreamSoundFile::FileHeader::HasRegionBlock() const {
    return GetReferenceBy(ElementType_StreamSoundFile_RegionBlock) != nullptr;
}

u32 StreamSoundFile::FileHeader::GetInfoBlockSize() const {
    return GetReferenceBy(ElementType_StreamSoundFile_InfoBlock)->size;
}

u32 StreamSoundFile::FileHeader::GetSeekBlockSize() const {
    return GetReferenceBy(ElementType_StreamSoundFile_SeekBlock)->size;
}

u32 StreamSoundFile::FileHeader::GetDataBlockSize() const {
    return GetReferenceBy(ElementType_StreamSoundFile_DataBlock)->size;
}

u32 StreamSoundFile::FileHeader::GetRegionBlockSize() const {
    return GetReferenceBy(ElementType_StreamSoundFile_RegionBlock)->size;
}

u32 StreamSoundFile::FileHeader::GetInfoBlockOffset() const {
    return GetReferenceBy(ElementType_StreamSoundFile_InfoBlock)->offset;
}

u32 StreamSoundFile::FileHeader::GetSeekBlockOffset() const {
    return GetReferenceBy(ElementType_StreamSoundFile_SeekBlock)->offset;
}

const Util::ReferenceWithSize* StreamSoundFile::FileHeader::GetReferenceBy(u16 typeId) const {
    for (int i {0}; i < dataBlocks; ++i) {
        const Util::ReferenceWithSize* p {&toBlocks[i]};
        if (p->typeId == typeId)
            return p;
    }

    return nullptr;
}
} // namespace nn::atk::detail