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

const Util::ReferenceTable& WaveSoundFile::WaveSoundData::GetTrackInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toTrackInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::ReferenceTable& WaveSoundFile::WaveSoundData::GetNoteInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toNoteInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const WaveSoundFile::TrackInfo& WaveSoundFile::WaveSoundData::GetTrackInfo(u32 index) const {
    const void* pTrackInfo;
    pTrackInfo = GetTrackInfoReferenceTable().GetReferedItem(index, ElementType_WaveSoundFile_TrackInfo);

    return *util::ConstBytePtr(pTrackInfo).Get<TrackInfo>();
}

const WaveSoundFile::NoteInfo& WaveSoundFile::WaveSoundData::GetNoteInfo(u32 index) const {
    const void* pNoteInfo;
    pNoteInfo = GetNoteInfoReferenceTable().GetReferedItem(index, ElementType_WaveSoundFile_NoteInfo);

    return *util::ConstBytePtr(pNoteInfo).Get<NoteInfo>();
}
} // namespace nn::atk::detail