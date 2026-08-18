#include <nn/atk/atk_BankFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
namespace {
const u8 DefaultOriginalKey {60};
const u8 DefaultVolume {127};
const u8 DefaultPan {64};
const float DefaultPitch {1.0};
const bool DefaultIgnoreNoteOff {false};
const u8 DefaultKeyGroup {0};
const u8 DefaultInterpolationType {0};
const AdshrCurve DefaultAdshrCurve {127, 127, 127, 127, 127};

enum VelocityRegionBitFlag {
    VelocityRegionBitFlag_Key = 0,
    VelocityRegionBitFlag_Volume = 1,
    VelocityRegionBitFlag_Pan = 2,
    VelocityRegionBitFlag_Pitch = 3,
    VelocityRegionBitFlag_InstrumentNoteParam = 4,
    VelocityRegionBitFlag_Sends = 8,
    VelocityRegionBitFlag_Envelope = 9,
    VelocityRegionBitFlag_Randomizer = 10,
    VelocityRegionBitFlag_Lfo = 11,
    VelocityRegionBitFlag_BasicParamFlag = 0b01000011111
};

struct DirectChunk {
    Util::Reference toRegion;

    const void* GetRegion() const {
        return util::ConstBytePtr(this, toRegion.offset).Get();
    }
};
static_assert(sizeof(DirectChunk) == 0x8);

struct RangeChunk {
    Util::Table<char> borderTable;

    const Util::Reference& GetRegionTableAddress(int index) const {
        return *util::ConstBytePtr(this, sizeof(borderTable.count) 
                                         + util::align_up(borderTable.count, 4) 
                                         + sizeof(Util::Reference) * index).Get<Util::Reference>();
    }

    const void* GetRegion(u32 index) const {
        bool isFoundRangeChunkIndex {false};
        u32 regionTableIndex {0};
        for (u32 i {0}; i < borderTable.count; ++i) {
            if (index <= borderTable.item[i]) {
                regionTableIndex = i;
                isFoundRangeChunkIndex = true;
                break;
            }
        }

        if (!isFoundRangeChunkIndex)
            return nullptr;

        const Util::Reference& ref {GetRegionTableAddress(regionTableIndex)};
        return util::ConstBytePtr(this, ref.offset).Get();
    }
};
static_assert(sizeof(RangeChunk) == 0x8);

struct IndexChunk {
    u8 min;
    u8 max;
    u8 reserved[2];
    Util::Reference toRegion[1];

    const void* GetRegion(u32 index) const {
        if (index >= min && index <= max)
            return util::ConstBytePtr(this, toRegion[index - min].offset).Get();

        return nullptr;
    }
};
static_assert(sizeof(IndexChunk) == 0xc);

enum RegionType {
    RegionType_Direct,
    RegionType_Range,
    RegionType_Index,
    RegionType_Unknown
};

RegionType GetRegionType(u16 typeId) {
    switch (typeId) {
    case ElementType_BankFile_DirectReferenceTable:
        return RegionType_Direct;

    case ElementType_BankFile_RangeReferenceTable:
        return RegionType_Range;

    case ElementType_BankFile_IndexReferenceTable:
        return RegionType_Index;

    default:
        return RegionType_Unknown;

    }
}

const void* GetDirectChunk(const void* regionChunk) {
    const DirectChunk& directChunk {*reinterpret_cast<const DirectChunk*>(regionChunk)};

    return directChunk.GetRegion();
}

const void* GetRangeChunk(const void* regionChunk, u32 index) {
    const RangeChunk& rangeChunk {*reinterpret_cast<const RangeChunk*>(regionChunk)};

    return rangeChunk.GetRegion(index);
}

const void* GetIndexChunk(const void* regionChunk, u32 index) {
    const IndexChunk& indexChunk {*reinterpret_cast<const IndexChunk*>(regionChunk)};
    
    return indexChunk.GetRegion(index);
}

const void* GetRegion(const void* startPtr, u16 typeId, u32 offset, u32 index) {
    const void* regionChunk {util::ConstBytePtr(startPtr, offset).Get()};
    const void* region {nullptr};

    switch (GetRegionType(typeId)) {
    case RegionType_Direct:
        region = GetDirectChunk(regionChunk);
        break;

    case RegionType_Range:
        region = GetRangeChunk(regionChunk, index);
        break;

    case RegionType_Index:
        region = GetIndexChunk(regionChunk, index);
        break;

    case RegionType_Unknown:
    default:
        region = nullptr;
        break;
    }

    return region;
}
} // anonymous namespace

const BankFile::InfoBlock* BankFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_BankFile_InfoBlock))
            .Get<InfoBlock>();
}

const Util::WaveIdTable& BankFile::InfoBlockBody::GetWaveIdTable() const {
    return *util::ConstBytePtr(this).Advance(toWaveIdTable.offset)
            .Get<Util::WaveIdTable>();
}

const Util::ReferenceTable& BankFile::InfoBlockBody::GetInstrumentReferenceTable() const {
    return *util::ConstBytePtr(this).Advance(toInstrumentReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const BankFile::Instrument* BankFile::InfoBlockBody::GetInstrument(int programNo) const {
    auto& table {GetInstrumentReferenceTable()};
    auto& ref {table.item[programNo]};

    if (ref.IsValidTypeId(ElementType_BankFile_InstrumentInfo))
        return util::ConstBytePtr(table.GetReferedItem(programNo)).Get<BankFile::Instrument>();

    return nullptr;
}

const BankFile::KeyRegion* BankFile::Instrument::GetKeyRegion(u32 key) const {
    return util::ConstBytePtr(
                GetRegion(this, 
                          toKeyRegionChunk.typeId, 
                          toKeyRegionChunk.offset, 
                          key)
            ).Get<KeyRegion>();
}

const BankFile::VelocityRegion* BankFile::KeyRegion::GetVelocityRegion(u32 velocity) const {
    return util::ConstBytePtr(
                GetRegion(this, 
                          toVelocityRegionChunk.typeId, 
                          toVelocityRegionChunk.offset, 
                          velocity)
            ).Get<VelocityRegion>();
}

u8 BankFile::VelocityRegion::GetOriginalKey() const {
    u32 value;
    bool result {optionParameter.GetValue(&value, VelocityRegionBitFlag_Key)};
    if (result)
        return value;

    return DefaultOriginalKey;
}
} // namespace nn::atk::detail