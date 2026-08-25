#include <nn/atk/atk_WaveSoundFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
namespace {
const u8 WsdDefaultPan {64};
const s8 WsdDefaultSurroundPan {0};
const float WsdDefaultPitch {1.0};
const u8 WsdDefaultMainSend {127};
const u8 WsdDefaultFxSend {0};
const AdshrCurve WsdDefaultAdshrCurve {
    127,
    127,
    127,
    127,
    127
};
const u8 WsdDefaultLpfFreq {64};
const u8 WsdDefaultBiquadType {0};
const u8 WsdDefaultBiquadValue {0};
const u8 WsdDefaultKey {64};
const u8 WsdDefaultVolume {96};

enum WaveSoundInfoBitFlagWsd {
    WaveSoundInfoBitFlagWsd_Pan = 0,
    WaveSoundInfoBitFlagWsd_Pitch = 1,
    WaveSoundInfoBitFlagWsd_Filter = 2,
    WaveSoundInfoBitFlagWsd_Send = 8,
    WaveSoundInfoBitFlagWsd_Envelope = 9,
    WaveSoundInfoBitFlagWsd_Randomizer = 10,
};

enum NoteInfoBitFlag {
    NoteInfoBitFlag_Key = 0,
    NoteInfoBitFlag_Volume = 1,
    NoteInfoBitFlag_Pan = 2,
    NoteInfoBitFlag_Pitch = 3,
    NoteInfoBitFlag_Send = 8,
    NoteInfoBitFlag_Envelope = 9,
    NoteInfoBitFlag_Randomizer = 10,
    NoteInfoBitFlag_Lfo = 11,
};

struct SendValueWsd {
    u8 mainSend;
    Util::Table<u8, u8> fxSend;
};
} // anonymous namespace

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

u8 WaveSoundFile::WaveSoundInfo::GetPan() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, WaveSoundInfoBitFlagWsd_Pan)};
    if (result)
        return Util::DivideBy8bit(value, 0);

    return WsdDefaultPan;
}

s8 WaveSoundFile::WaveSoundInfo::GetSurroundPan() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, WaveSoundInfoBitFlagWsd_Pan)};
    if (result)
        return static_cast<s8>(Util::DivideBy8bit(value, 1));

    return WsdDefaultSurroundPan;
}
} // namespace nn::atk::detail