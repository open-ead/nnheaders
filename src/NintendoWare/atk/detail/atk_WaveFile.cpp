#include <nn/atk/atk_WaveFile.h>

#include <nn/atk/atk_ElementType.h>
#include "nn/util/util_BytePtr.h"

namespace nn::atk::detail {
const WaveFile::InfoBlock* WaveFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_WaveFile_InfoBlock))
        .Get<WaveFile::InfoBlock>();
}

const WaveFile::DataBlock* WaveFile::FileHeader::GetDataBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_WaveFile_DataBlock))
        .Get<WaveFile::DataBlock>();
}

const WaveFile::ChannelInfo& WaveFile::InfoBlockBody::GetChannelInfo(int channelIndex) const {
    return *util::ConstBytePtr(channelInfoReferenceTable.GetReferedItem(channelIndex))
            .Get<WaveFile::ChannelInfo>();
}

const void* WaveFile::ChannelInfo::GetSamplesAddress(const void* dataBlockBodyAddress) const {
    return util::ConstBytePtr(dataBlockBodyAddress).Advance(referToSamples.offset).Get();
}

const WaveFile::DspAdpcmInfo& WaveFile::ChannelInfo::GetDspAdpcmInfo() const {
    return *util::ConstBytePtr(this).Advance(referToAdpcmInfo.offset)
            .Get<WaveFile::DspAdpcmInfo>();
}
} // namespace nn::atk::detail