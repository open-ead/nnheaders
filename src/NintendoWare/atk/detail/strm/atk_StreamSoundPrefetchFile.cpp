#include <nn/atk/atk_StreamSoundPrefetchFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const StreamSoundFile::InfoBlock* StreamSoundPrefetchFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_StreamSoundFile_InfoBlock))
            .Get<StreamSoundFile::InfoBlock>();
}

const StreamSoundFile::RegionBlock* StreamSoundPrefetchFile::FileHeader::GetRegionBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_StreamSoundFile_RegionBlock))
            .Get<StreamSoundFile::RegionBlock>();
}

const StreamSoundPrefetchFile::PrefetchDataBlock* StreamSoundPrefetchFile::FileHeader::GetPrefetchDataBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_StreamSoundFile_PrefetchDataBlock)).Get<PrefetchDataBlock>();
}

u32 StreamSoundPrefetchFile::FileHeader::GetPrefetchDataBlockSize() const {
    return GetBlockSize(ElementType_StreamSoundFile_PrefetchDataBlock);
}

bool StreamSoundPrefetchFile::FileHeader::HasRegionBlock() const {
    return GetBlock(ElementType_StreamSoundFile_RegionBlock) != nullptr;
}

u32 StreamSoundPrefetchFile::FileHeader::GetRegionBlockSize() const {
    return GetBlockSize(ElementType_StreamSoundFile_RegionBlock);
}
} // namespace nn::atk::detail