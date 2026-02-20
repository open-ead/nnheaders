#pragma once

#include <nn/util.h>

#include <nn/atk/atk_Global.h>
#include <nn/atk/detail/atk_BinaryFileFormat.h>

namespace nn::atk {
class SoundArchive;

namespace detail {
struct Util {
    NN_NO_COPY(Util);
    NN_NO_MOVE(Util);

    constexpr static u16 CalcLpfFreqTable[24] {
        80, 100, 128, 160, 
        200, 256, 320, 400, 
        500, 640, 800, 1000,
        1280, 1600, 2000, 2560, 
        3200, 4000, 5120, 6400, 
        8000, 10240, 12800, 16000
    };

    struct Reference {
        constexpr static s32 InvalidOffset = -1;

        // Id from ElementTypes enum
        u16 typeId;
        u8 padding[2];
        s32 offset;
    };
    static_assert(sizeof(Reference) == 8);

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
            struct Element {
                s32 logId;
                s32 arg0;
                s32 arg1;
            };
            static_assert(sizeof(Element) == 0xc);

            Element element[64];
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

    u16 CalcLpfFreq(float scale);
    BiquadFilterCoefficients CalcLowPassFilterCoefficients(s32 frequency, s32 sampleRate, 
                                                           bool isTableUsed);
    float CalcPanRatio(float pan, PanInfo* info, OutputMode mode);
    float CalcSurroundPanRatio(float surroundPan, PanInfo* info);
    float CalcPitchRatio(float pitch_);
    float CalcVolumeRatio(float dB);
    float CalcRandom();
    
    WaveArchiveLoadStatus GetWaveArchiveOfBank(class LoadItemInfo* warcLoadInfo, 
                                               bool* isLoadIndividual, void* bankFile, 
                                               SoundArchive* arc, class SoundArchiveLoader* mgr);
    void* GetWaveFileOfWaveSound(void* wsdFile, u32 index, SoundArchive* arc, SoundArchiveLoader* mgr);
    size_t GetByteBySample(size_t samples, SampleFormat format);
    size_t GetSampleByByte(size_t samples, SampleFormat format);
    
    bool IsValidMemoryForDsp(void* ptr, size_t size);
};

} // namespace nn::atk::detail
} // namespace nn::atk