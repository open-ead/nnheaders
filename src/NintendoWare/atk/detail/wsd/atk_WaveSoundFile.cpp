#include <nn/atk/atk_WaveSoundFile.h>

#include <nn/atk/atk_ElementType.h>
#include "nn/util/util_BytePtr.h"

namespace nn::atk::detail {
const WaveSoundFile::InfoBlock* WaveSoundFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_WaveSoundFile_InfoBlock))
            .Get<WaveSoundFile::InfoBlock>();
}

const WaveSoundFile::WaveSoundData& WaveSoundFile::InfoBlockBody::GetWaveSoundData(u32 index) const {
    return *util::ConstBytePtr(
                GetWaveSoundDataReferenceTable()
                .GetReferedItem(index, ElementType_WaveSoundFile_WaveSoundMetaData)
            ).Get<WaveSoundData>();
}

const Util::ReferenceTable& WaveSoundFile::InfoBlockBody::GetWaveSoundDataReferenceTable() const {
    return *util::ConstBytePtr(this, toWaveSoundDataReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::WaveIdTable& WaveSoundFile::InfoBlockBody::GetWaveIdTable() const {
    return *util::ConstBytePtr(this, toWaveIdTable.offset).Get<Util::WaveIdTable>();
}

const WaveSoundFile::WaveSoundInfo& WaveSoundFile::WaveSoundData::GetWaveSoundInfo() const {
    return *util::ConstBytePtr(this, toWaveSoundInfo.offset).Get<WaveSoundInfo>();
}
} // namespace nn::atk::detail