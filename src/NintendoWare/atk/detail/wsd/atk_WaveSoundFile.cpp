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

float WaveSoundFile::WaveSoundInfo::GetPitch() const {
    float value;
    bool result {optionParameter.GetValueF32(&value, WaveSoundInfoBitFlagWsd_Pitch)};
    if (result)
        return value;

    return WsdDefaultPitch;
}

void WaveSoundFile::WaveSoundInfo::GetSendValue(u8* mainSend, u8* fxSend, u8 fxSendCount) const {
    u32 value;
    bool result {optionParameter.GetValue(&value, WaveSoundInfoBitFlagWsd_Send)};

    if (result) {
        const SendValueWsd& sendValue = *util::ConstBytePtr(this, value).Get<SendValueWsd>();

        *mainSend = sendValue.mainSend;
        int countSize {sendValue.fxSend.count > AuxBus_Count ? AuxBus_Count : sendValue.fxSend.count};

        for (int i {0}; i < countSize; ++i)
            fxSend[i] = sendValue.fxSend.item[i];
    }
    else {
        *mainSend = WsdDefaultMainSend;
        for (int i {0}; i < fxSendCount; ++i)
            fxSend[i] = WsdDefaultFxSend;
    }
}

const AdshrCurve& WaveSoundFile::WaveSoundInfo::GetAdshrCurve() const {
    u32 offsetToReference;
    bool result {optionParameter.GetValue(&offsetToReference, WaveSoundInfoBitFlagWsd_Envelope)};
    if (result) {
        const auto& ref {*util::ConstBytePtr(this, offsetToReference).Get<Util::Reference>()};
        return *util::ConstBytePtr(&ref, ref.offset).Get<AdshrCurve>();
    }

    return WsdDefaultAdshrCurve;
}

u8 WaveSoundFile::WaveSoundInfo::GetLpfFreq() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, WaveSoundInfoBitFlagWsd_Filter)};
    if (result)
        return Util::DivideBy8bit(value, 0);

    return WsdDefaultLpfFreq;
}

u8 WaveSoundFile::WaveSoundInfo::GetBiquadType() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, WaveSoundInfoBitFlagWsd_Filter)};
    if (result)
        return Util::DivideBy8bit(value, 1);

    return WsdDefaultBiquadType;
}

u8 WaveSoundFile::WaveSoundInfo::GetBiquadValue() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, WaveSoundInfoBitFlagWsd_Filter)};
    if (result)
        return Util::DivideBy8bit(value, 2);

    return WsdDefaultBiquadValue;
}

const Util::ReferenceTable& WaveSoundFile::TrackInfo::GetNoteEventReferenceTable() const {
    return *util::ConstBytePtr(this, toNoteEventReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const WaveSoundFile::NoteEvent& WaveSoundFile::TrackInfo::GetNoteEvent(u32 index) const {
    const void* pNoteEvent;
    pNoteEvent = GetNoteEventReferenceTable().GetReferedItem(index, ElementType_WaveSoundFile_NoteEvent);

    return *util::ConstBytePtr(pNoteEvent).Get<NoteEvent>();
}

u8 WaveSoundFile::NoteInfo::GetOriginalKey() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, NoteInfoBitFlag_Key)};
    if (result)
        return Util::DivideBy8bit(value, 0);

    return WsdDefaultKey;
}

u8 WaveSoundFile::NoteInfo::GetVolume() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, NoteInfoBitFlag_Volume)};
    if (result)
        return Util::DivideBy8bit(value, 0);

    return WsdDefaultVolume;
}

u8 WaveSoundFile::NoteInfo::GetPan() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, NoteInfoBitFlag_Pan)};
    if (result)
        return Util::DivideBy8bit(value, 0);

    return WsdDefaultPan;
}

u8 WaveSoundFile::NoteInfo::GetSurroundPan() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, NoteInfoBitFlag_Pan)};
    if (result)
        return Util::DivideBy8bit(value, 1);

    return WsdDefaultSurroundPan;
}

float WaveSoundFile::NoteInfo::GetPitch() const {
    float value;
    bool result {optionParameter.GetValueF32(&value, NoteInfoBitFlag_Pitch)};
    if (result)
        return value;

    return WsdDefaultPitch;
}

const AdshrCurve& WaveSoundFile::NoteInfo::GetAdshrCurve() const {
    u32 offsetToReference;
    bool result {optionParameter.GetValue(&offsetToReference, NoteInfoBitFlag_Envelope)};
    if (result) {
        const auto& ref {*util::ConstBytePtr(this, offsetToReference).Get<Util::Reference>()};
        return *util::ConstBytePtr(&ref, ref.offset).Get<AdshrCurve>();
    }

    return WsdDefaultAdshrCurve;
}
} // namespace nn::atk::detail