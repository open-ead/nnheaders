#pragma once

#include <nn/util.h>
#include <nn/util/util_BytePtr.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/atk_BinaryFileFormat.h>

namespace nn::atk {
class SoundArchive;
class SoundArchivePlayer;
class OutputReceiver;

namespace detail {
class PlayerHeapDataManager;
class SoundArchiveLoader;
struct LoadItemInfo;
struct Util {
    NN_NO_COPY(Util);
    NN_NO_MOVE(Util);

    constexpr static s32 VolumeDbMin = -904;
    constexpr static s32 VolumeDbMax = 60;

    constexpr static s32 PitchDivisionBit = 0b1000;
    constexpr static s32 PitchDivisionRange = 256;
    
    constexpr static s32 CalcLpfFreqTableSize = 24;
    constexpr static u32 CalcLpfFreqIntercept = 0x3E0ADE7F;
    constexpr static u32 CalcLpfFreqThreshold = 0x3F666666;

    constexpr static u16 CalcLpfFreqTable[CalcLpfFreqTableSize] {
        80, 100, 128, 160, 
        200, 256, 320, 400, 
        500, 640, 800, 1000,
        1280, 1600, 2000, 2560, 
        3200, 4000, 5120, 6400, 
        8000, 10240, 12800, 16000
    };

    constexpr static BiquadFilterCoefficients LowPassFilterCoefficientsTable32000[CalcLpfFreqTableSize] {};
    constexpr static BiquadFilterCoefficients LowPassFilterCoefficientsTable48000[CalcLpfFreqTableSize] {};

    template <typename Class>
    class Singleton {
    public:
        static Class* GetInstance(); 
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

    template <typename ItemType, typename CountType = u32>
    struct Table {
        CountType count;
        ItemType item[1];
    };

    struct ReferenceTable : Table<Reference> {
        const void* GetReferedItem(u32 index) const {
            if (count > index)
                return util::ConstBytePtr(this, item[index].offset).Get();

            return nullptr;
        }

        const void* GetReferedItem(u32 index, u16 typeId) const {
            if (count > index && item[index].typeId == typeId)
                return util::ConstBytePtr(this,item[index].offset).Get();

            return nullptr;
        }

        const void* FindReferedItemBy(u16 typeId) const;
    };

    struct ReferenceWithSizeTable : Table<ReferenceWithSize> {
        const void* GetReferedItem(u32 index) const {
            if (count > index)
                return util::ConstBytePtr(this, item[index].offset).Get();

            return nullptr;
        }

        const void* GetReferedItemBy(u16 typeId) const;
        u32 GetReferedItemSize(u32 index) const;
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

    enum WaveArchiveLoadStatus {
        WaveArchiveLoadStatus_Error = -2,
        WaveArchiveLoadStatus_NotYet,
        WaveArchiveLoadStatus_Ok,
        WaveArchiveLoadStatus_Noneed,
        WaveArchiveLoadStatus_Partly
    };

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
    };

    class WarningLogger {
    public:
        enum LogId {
            LogId_ChannelAllocationFailed,
            LogId_SoundthreadFailedWakeup,
            LogId_LogbufferFull,
            LogId_Max,
        };

        struct LogBuffer {
            constexpr static u32 LogCount = 64;

            struct Element {
                
                void Print();

                s32 logId;
                s32 arg0;
                s32 arg1;
            };
            static_assert(sizeof(Element) == 0xc);

            void Log(s32 logId, s32 arg0, s32 arg1);
            void Print();

            Element element[LogCount];
            s32 counter;
        };
        static_assert(sizeof(LogBuffer) == 0x304);

        void Log(s32 logId, s32 arg0, s32 arg1);
        void SwapBuffer();
        void Print();

    private:
        LogBuffer m_Buffer0;
        LogBuffer m_Buffer1;
        LogBuffer* m_pCurrentBuffer;
    };
    static_assert(sizeof(WarningLogger) == 0x610);

    static u16 CalcLpfFreq(f32 scale);
    static BiquadFilterCoefficients CalcLowPassFilterCoefficients(s32 frequency, s32 sampleRate, 
                                                           bool isTableUsed);

    static s32 FindLpfFreqTableIndex(s32);

    static f32 CalcPanRatio(f32 pan, PanInfo* info, OutputMode mode);
    static f32 CalcSurroundPanRatio(f32 surroundPan, PanInfo* info);
    static f32 CalcPitchRatio(f32 pitch_);
    static f32 CalcVolumeRatio(f32 dB);
    static f32 CalcRandom();
    
    static void* GetWaveFile(u32, u32, const SoundArchive&, const SoundArchivePlayer&);
    static void* GetWaveFile(u32, u32, const SoundArchive&, const PlayerHeapDataManager&);

    static WaveArchiveLoadStatus GetWaveArchiveOfBank(const LoadItemInfo& warcLoadInfo, 
                                                      bool& isLoadIndividual, const void* bankFile, 
                                                      const SoundArchive& arc, const SoundArchiveLoader& mgr);

    static void* GetWaveFileOfWaveSound(const void* wsdFile, u32 index, 
                                        const SoundArchive& arc, const SoundArchiveLoader& mgr);

    static s32 GetOutputReceiverMixBufferIndex(OutputReceiver*, s32, s32);

    static size_t GetSampleByByte(size_t samples, SampleFormat format);
    static size_t GetByteBySample(size_t samples, SampleFormat format);
    
    static bool IsValidMemoryForDsp(const void* ptr, size_t size);
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