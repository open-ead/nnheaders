#pragma once

#include <nn/util.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/detail/util/atk_BinaryFileFormat.h>

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
        constexpr static s32 InvalidOffset = -1;

        // Id from ElementTypes enum
        u16 typeId;
        u8 padding[2];
        s32 offset;
    };
    static_assert(sizeof(Reference) == 0x8);

    struct ReferenceWithSize : Reference {
        u32 size;
    };
    static_assert(sizeof(ReferenceWithSize) == 0xc);

    struct BlockReferenceTable {
        ReferenceWithSize item[1];
    };

    struct SoundFileHeader {
        BinaryFileHeader header;
        BlockReferenceTable blockReferenceTable;
    };

    template <typename T, typename CountType = u32>
    struct Table {
        CountType count;
        T item[1];
    };

    struct ReferenceTable : Table<Reference> {};
    struct ReferenceWithSizeTable : Table<ReferenceWithSize> {};

    struct BitFlag {
        constexpr static u32 BitNumberMax = 31;

        u32 bitFlag;
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