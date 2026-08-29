#pragma once

#include <nn/util.h>
#include <nn/util/util_BytePtr.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_BinaryFileFormat.h>
#include <nn/atk/atk_ItemType.h>

namespace nn::atk {
class SoundArchive;
class SoundArchivePlayer;
class OutputReceiver;

namespace detail {
class PlayerHeapDataManager;
class SoundArchiveLoader;
struct LoadItemInfo;
class Util {
public:
    static const int VolumeDbMin {-904};
    static const int VolumeDbMax {60};

    static const int PitchDivisionBit {8};
    static const int PitchDivisionRange {256};
    
    enum PanCurve {
        PanCurve_Sqrt,
        PanCurve_Sincos,
        PanCurve_Linear,
    };

    struct PanInfo {
        PanCurve curve;
        bool centerZeroFlag;
        bool zeroClampFlag;
        bool isEnableFrontBypass;
    };
    static_assert(sizeof(PanInfo) == 0x8);

    static u16 CalcLpfFreq(float scale);
    static BiquadFilterCoefficients CalcLowPassFilterCoefficients(int frequency, 
                                                                  int sampleRate, 
                                                                  bool isTableUsed);
    static int FindLpfFreqTableIndex(int frequency);
    static float CalcPanRatio(float pan, const PanInfo& info, OutputMode mode);
    static float CalcSurroundPanRatio(float surroundPan, const PanInfo& info);

    static float CalcPitchRatio(int pitch_);
    static float CalcVolumeRatio(float dB);
    static u16 CalcRandom();

    static size_t GetSampleByByte(size_t byte, SampleFormat format);
    static size_t GetByteBySample(size_t samples, SampleFormat format);

    static bool IsValidMemoryForDsp(const void* ptr, size_t size);

    static const int CalcLpfFreqTableSize {24};
    static const float CalcLpfFreqIntercept; // = 0x3E0ADE7F
    static const float CalcLpfFreqThreshold;// = 0x3F666666;
    static const u16 CalcLpfFreqTable[CalcLpfFreqTableSize];
    static const BiquadFilterCoefficients LowPassFilterCoefficientsTable32000[CalcLpfFreqTableSize];
    static const BiquadFilterCoefficients LowPassFilterCoefficientsTable48000[CalcLpfFreqTableSize];

    template <typename ItemType, typename CountType = u32>
    struct Table {
        CountType count;
        ItemType item[1];
    };

    struct Reference {
        static const int InvalidOffset = -1;

        bool IsValidTypeId(u16 validId) const { return typeId == validId; }
        bool IsValidOffset() const { return offset != InvalidOffset; }

        // Id from ElementType enum
        u16 typeId;
        u8 padding[2];
        s32 offset;
    };
    static_assert(sizeof(Reference) == 0x8);

    struct ReferenceWithSize : public Reference {
        u32 size;
    };
    static_assert(sizeof(ReferenceWithSize) == 0xc);

    struct ReferenceTable : Table<Reference> {
        const void* GetReferedItem(u32 index) const {
            if (count <= index)
                return nullptr;
            
            return util::ConstBytePtr(this, item[index].offset).Get();
        }

        const void* GetReferedItem(u32 index, u16 typeId) const {
            if (count <= index || item[index].typeId != typeId)
                return nullptr;
            
            return util::ConstBytePtr(this,item[index].offset).Get();
        }

        const void* FindReferedItemBy(u16 typeId) const;
    };

    struct ReferenceWithSizeTable : Table<ReferenceWithSize> {
        const void* GetReferedItem(u32 index) const {
            if (count <= index)
                return nullptr;
            
            return util::ConstBytePtr(this, item[index].offset).Get();
        }

        const void* GetReferedItemBy(u16 typeId) const;
        u32 GetReferedItemSize(u32 index) const;
    };

    struct BlockReferenceTable {
        ReferenceWithSize item[1];

        const void* GetReferedItemByIndex(const void* origin, int index, u16 count) const;
        
        const ReferenceWithSize* GetReference(u16 typeId, u16 count) const {
            for (int i {0}; i < count; ++i) {
                if (item[i].IsValidTypeId(typeId))
                    return &item[i];
            }

            return nullptr;
        }

        const void* GetReferedItem(const void* origin, u16 typeId, u16 count) const {
            auto* ref {GetReference(typeId, count)};
            if (ref != nullptr && ref->offset != 0)
                return util::ConstBytePtr(origin, ref->offset).Get();

            return nullptr;
        }

        u32 GetReferedItemSize(u16 typeId, u16 count) const {
            auto* ref {GetReference(typeId, count)};
            if (ref != nullptr)
                return ref->size;

            return 0;
        }
        
        u32 GetReferedItemOffset(u16 typeId, u16 count) const {
            auto* ref {GetReference(typeId, count)};
            if (ref != nullptr)
                return ref->offset;

            return 0;
        }
    };

    struct SoundFileHeader {
        BinaryFileHeader header;
        BlockReferenceTable blockReferenceTable;

        int GetBlockCount() const { return header.dataBlocks; }

    protected:
        const void* GetBlock(u16 typeId) const {
            return blockReferenceTable.GetReferedItem(this, typeId, header.dataBlocks);
        }

        u32 GetBlockSize(u16 typeId) const {
            return blockReferenceTable.GetReferedItemSize(typeId, header.dataBlocks);
        }

        u32 GetBlockOffset(u16 typeId) const {
            return blockReferenceTable.GetReferedItemOffset(typeId, header.dataBlocks);
        }
    };

    struct BitFlag {
        u32 bitFlag;

        bool GetValue(u32* value, u32 bitNumber) const {
            u32 count {GetTrueCount(bitNumber)};
            if (count == 0)
                return false;

            *value = (&bitFlag)[count];
            return true;
        }

        bool GetValueF32(float* value, u32 bitNumber) const {
            u32 count {GetTrueCount(bitNumber)};
            if (count == 0)
                return false;

            *value = reinterpret_cast<const float*>(&bitFlag)[count];
            return true;
        }

    private:
        static const int BitNumberMax = 31;
        u32 GetTrueCount(u32 bitNumber) const {
            int count {0};

            bool ret {false};
            for (u32 i {0}; i <= bitNumber; ++i) {
                if ((bitFlag & (1 << i)) != 0) {
                    ++count;
                    if (i == bitNumber)
                        ret = true;
                }
            }

            if (ret)
                return count;

            return 0;
        }
    };
    static_assert(sizeof(BitFlag) == 0x4);

    static u8 DivideBy8bit(u32 value, int index) {
        return static_cast<u8>(value >> (8 * index));
    }

    static u8 DivideBy16bit(u32 value, int index) {
        return static_cast<u16>(value >> (16 * index));
    }

    const void* GetWaveFile(u32 waveArchiveId, u32 waveIndex, 
                            const SoundArchive& arc, const SoundArchivePlayer& player);
    const void* GetWaveFile(u32 waveArchiveId, u32 waveIndex, 
                            const SoundArchive& arc, const PlayerHeapDataManager* mgr);

    enum WaveArchiveLoadStatus {
        WaveArchiveLoadStatus_Error = -2,
        WaveArchiveLoadStatus_NotYet,
        WaveArchiveLoadStatus_Ok,
        WaveArchiveLoadStatus_Noneed,
        WaveArchiveLoadStatus_Partly
    };

    static WaveArchiveLoadStatus GetWaveArchiveOfBank(LoadItemInfo& warcLoadInfo, 
                                                      bool& isLoadIndividual, 
                                                      const void* bankFile, 
                                                      const SoundArchive& arc, 
                                                      const SoundArchiveLoader& mgr);

    static const void* GetWaveFileOfWaveSound(const void* wsdFile, 
                                              u32 index, 
                                              const SoundArchive& arc, 
                                              const SoundArchiveLoader& mgr);

    static ItemType GetItemType(u32 id) {
        return static_cast<ItemType>(id >> 24);
    }

    static u32 GetItemIndex(u32 id) {
        return id & 0x00FFFFFF;
    }

    static u32 GetMaskedItemId(u32 id, ItemType type) {
        return id | (type << 24);
    }
    
    struct WaveId {
        u32 waveArchiveId;
        u32 waveIndex;
    };
    static_assert(sizeof(WaveId) == 0x8);
    
    struct WaveIdTable {
        Table<WaveId> table;

        const WaveId* GetWaveId(u32 index) const {
            return &table.item[index];
        }

        u32 GetCount() const {
            return table.count;
        }
    };

    template <typename CHILD>
    class Singleton {
    public:
        static CHILD& GetInstance(); 
    };

    static int GetSubMixBusFromMainBus();
    static int GetSubMixBus(AuxBus bus);
    static int GetSubMixBus(int bus);
    static int GetOutputReceiverMixBufferIndex(const OutputReceiver* pOutputReceiver, int bus, int channel);
    static int GetAdditionalSendIndex(int bus);

    class WarningLogger : public Singleton<WarningLogger> {
    public:
        WarningLogger();

        void Log(int logId, int arg0, int arg1);
        void Print();
        void SwapBuffer();

    private:
        struct LogBuffer {
            static const int LogCount = 64;

            struct Element {
                int logId;
                int arg0;
                int arg1;

                void Print();
            };
            static_assert(sizeof(Element) == 0xc);


            Element element[LogCount];
            int counter;

            LogBuffer();

            void Log(int logId, int arg0, int arg1);
            void Print();
            void Reset();
        };
        static_assert(sizeof(LogBuffer) == 0x304);

        LogBuffer m_Buffer0;
        LogBuffer m_Buffer1;
        LogBuffer* m_pCurrentBuffer;
    };
    static_assert(sizeof(WarningLogger) == 0x610);
};

static const f32 NoteTable[12] {};
static const f32 PitchTable[Util::PitchDivisionRange] {};
static const f32 Decibel2RatioTable[965] {};

static const f32 Pan2RatioTableSqrtSurround[257] {};
static const f32 Pan2RatioTableSinCosSurround[257] {};
static const f32 Pan2RatioTableLinearSurround[257] {};

static const f32 Pan2RatioTableSqrt[257] {};
static const f32 Pan2RatioTableSinCos[257] {};
static const f32 Pan2RatioTableLinear[257] {};

static const f32* PanTableTable[3] {Pan2RatioTableSqrt, Pan2RatioTableSinCos, Pan2RatioTableLinear};
static const f32* PanTableTableForSurround[3] {Pan2RatioTableSqrtSurround, Pan2RatioTableSinCosSurround, Pan2RatioTableLinearSurround};
} // namespace nn::atk::detail
} // namespace nn::atk