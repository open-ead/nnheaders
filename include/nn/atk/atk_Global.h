#pragma once

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/detail/atk_Config.h>

namespace nn::atk {
enum WaveType {
    WaveType_Invalid = -1,
    WaveType_Nwwav,
    WaveType_Dspadpcm,
};

struct WaveBuffer {
    enum Status {
        Status_Free,
        Status_Wait,
        Status_Play,
        Status_Done,
    };

    void* bufferAddress;
    size_t bufferSize;
    size_t sampleLength;
    position_t sampleOffset;
    AdpcmContext* pAdpcmContext;
    void* userParam;
    bool loopFlag;
    Status status;
    WaveBuffer* next;
};
static_assert(sizeof(WaveBuffer) == 0x40);

enum AuxBus {
    AuxBus_A,
    AuxBus_B,
    AuxBus_C,
    AuxBus_Count,
};

enum BiquadFilterType {
    BiquadFilterType_Inherit = -1,
    BiquadFilterType_Min = -1,
    BiquadFilterType_DataMin = 0,

    BiquadFilterType_None = 0,
    BiquadFilterType_LowPassFilter,
    BiquadFilterType_HighPassFilter,
    BiquadFilterType_BandPassFilter512,
    BiquadFilterType_BandPassFilter1024,
    BiquadFilterType_BandPassFilter2048,
    BiquadFilterType_LowPassFilterNw4fCompatible48k,
    BiquadFilterType_HighPassFilterNw4fCompatible48k,
    BiquadFilterType_BandPassFilter512Nw4fCompatible48k,
    BiquadFilterType_BandPassFilter1024Nw4fCompatible48k,
    BiquadFilterType_BandPassFilter2048Nw4fCompatible48k,

    BiquadFilterType_UserMin = 0x40,

    BiquadFilterType_User0 = BiquadFilterType_UserMin,
    BiquadFilterType_User1,
    BiquadFilterType_User2,
    BiquadFilterType_User3,
    BiquadFilterType_User4,
    BiquadFilterType_User5,
    BiquadFilterType_User6,
    BiquadFilterType_User7,
    BiquadFilterType_User8,
    BiquadFilterType_User9,
    BiquadFilterType_User10,
    BiquadFilterType_User11,
    BiquadFilterType_User12,
    BiquadFilterType_User13,
    BiquadFilterType_User14,
    BiquadFilterType_User15,
    BiquadFilterType_User16,
    BiquadFilterType_User17,
    BiquadFilterType_User18,
    BiquadFilterType_User19,
    BiquadFilterType_User20,
    BiquadFilterType_User21,
    BiquadFilterType_User22,
    BiquadFilterType_User23,
    BiquadFilterType_User24,
    BiquadFilterType_User25,
    BiquadFilterType_User26,
    BiquadFilterType_User27,
    BiquadFilterType_User28,
    BiquadFilterType_User29,
    BiquadFilterType_User30,
    BiquadFilterType_User31,
    BiquadFilterType_User32,
    BiquadFilterType_User33,
    BiquadFilterType_User34,
    BiquadFilterType_User35,
    BiquadFilterType_User36,
    BiquadFilterType_User37,
    BiquadFilterType_User38,
    BiquadFilterType_User39,
    BiquadFilterType_User40,
    BiquadFilterType_User41,
    BiquadFilterType_User42,
    BiquadFilterType_User43,
    BiquadFilterType_User44,
    BiquadFilterType_User45,
    BiquadFilterType_User46,
    BiquadFilterType_User47,
    BiquadFilterType_User48,
    BiquadFilterType_User49,
    BiquadFilterType_User50,
    BiquadFilterType_User51,
    BiquadFilterType_User52,
    BiquadFilterType_User53,
    BiquadFilterType_User54,
    BiquadFilterType_User55,
    BiquadFilterType_User56,
    BiquadFilterType_User57,
    BiquadFilterType_User58,
    BiquadFilterType_User59,
    BiquadFilterType_User60,
    BiquadFilterType_User61,
    BiquadFilterType_User62,
    BiquadFilterType_User63,

    BiquadFilterType_Max = 0x7f,
    BiquadFilterType_UserMax = 0x7f,
};

enum ChannelIndex {
    ChannelIndex_FrontLeft,
    ChannelIndex_FrontRight,
    ChannelIndex_RearLeft,
    ChannelIndex_RearRight,
    ChannelIndex_FrontCenter,
    ChannelIndex_Lfe,
    ChannelIndex_Count,
};

enum CircularBufferSinkState {
    CircularBufferSinkState_Invalid,
    CircularBufferSinkState_Started,
    CircularBufferSinkState_Stopped,
};

enum FsPriority {
    FsPriority_RealTime,
    FsPriority_Normal,
    FsPriority_Low,
};

class AdshrCurve {
private:
    u8 m_Attack;
    u8 m_Decay;
    u8 m_Sustain;
    u8 m_Hold;
    u8 m_Release;
};
static_assert(sizeof(AdshrCurve) == 0x5);

struct BiquadFilterCoefficients {
    s16 b0;
    s16 b1;
    s16 b2;
    s16 a1;
    s16 a2;
};
static_assert(sizeof(BiquadFilterCoefficients) == 0xa);

struct DspAdpcmParam {
    u16 coef[8][2];
    u16 predScale;
    u16 yn1;
    u16 yn2;
};
static_assert(sizeof(DspAdpcmParam) == 0x26);

enum MixMode {
    MixMode_Pan = 0,
    MixMode_MixVolume = 1,
    MixMode_Mixparameter = 1,
    MixMode_Count = 2,
};

struct MixParameter {
    union {
        struct {
            f32 fL;
            f32 fR;
            f32 rL;
            f32 rR;
            f32 fC;
            f32 lfe;
        };
        float ch[6];
    };
};
static_assert(sizeof(MixParameter) == 0x18);

struct MixVolume {
    union {
        struct {
            f32 frontLeft;
            f32 frontRight;
            f32 rearLeft;
            f32 rearRight;
            f32 frontCenter;
            f32 lowFrequencyEffect;
        };
        float channel[6];
    };
};
static_assert(sizeof(MixVolume) == 0x18);

enum OutputDevice {
    OutputDevice_Main,
    OutputDevice_Count,
};

enum OutputDeviceIndex {
    OutputDeviceIndex_Main,
    OutputDeviceIndex_Count,
};

enum OutputLine {
    OutputLine_Main = 1,
    OutputLine_ReservedMax = 2,

    OutputLine_User0 = 0x10000,
    OutputLine_User1 = 0x20000,
    OutputLine_User2 = 0x40000,
    OutputLine_User3 = 0x80000,
};

enum OutputLineIndex {
    OutputLineIndex_Main = 1,
    OutputLineIndex_ReservedMax = 2,

    OutputLineIndex_User0 = 0x10000,
    OutputLineIndex_User1 = 0x20000,
    OutputLineIndex_User2 = 0x40000,
    OutputLineIndex_User3 = 0x80000,
};

enum OutputMode {
    OutputMode_Monaural,
    OutputMode_Stereo,
    OutputMode_Surround,
    OutputMode_Dpl2,
    OutputMode_Count,
};

struct OutputMix {
    float channelGain[24];
};
static_assert(sizeof(OutputMix) == 0x60);

enum PanCurve {
    PanCurve_Sqrt,
    PanCurve_Sqrt0Db,
    PanCurve_Sqrt0DbClamp,
    PanCurve_Sincos,
    PanCurve_Sincos0Db,
    PanCurve_Sincos0DbClamp,
    PanCurve_Linear,
    PanCurve_Linear0Db,
    PanCurve_Linear0DbClamp,
    PanCurve_Invalid,
};

enum PanMode {
    PanMode_Dual,
    PanMode_Balance,
    PanMode_Invalid,
};

enum PauseMode {
    PauseMode_Default = 0,

    PauseMode_Nw4fSndCompatible = PauseMode_Default,
    PauseMode_PauseImmediately,
};

enum SampleFormat {
    SampleFormat_PcmS8,
    SampleFormat_PcmS16,
    SampleFormat_DspAdpcm,
    SampleFormat_PcmS32,
};

enum SampleRateConverterType {
    SampleRateConverterType_None,
    SampleRateConverterType_Linear,
    SampleRateConverterType_4Tap,
};

enum SequenceMute {
    SequenceMute_Off,
    SequenceMute_NoStop,
    SequenceMute_Release,
    SequenceMute_Stop,
};

enum SinglePlayType {
    SinglePlayType_None,
    SinglePlayType_PrioritizeOldest,
    SinglePlayType_PrioritizeOldestEffectiveDuration,
    SinglePlayType_PrioritizeOldestWithDuration = SinglePlayType_PrioritizeOldestEffectiveDuration,
    
    SinglePlayType_PrioritizeNewest,
    SinglePlayType_PrioritizeNewestEffectiveDuration,
    SinglePlayType_PrioritizeNewestWithDuration = SinglePlayType_PrioritizeNewestEffectiveDuration,
};

enum UpdateType {
    UpdateType_AudioFrame,
    UpdateType_GameFrame,
};

enum VolumeThroughModeBitFlag {
    VolumeThroughMode_Binary = 1,
};

namespace detail {
    enum DecodeMode {
        DecodeMode_Invalid = -1,
        DecodeMode_Default,
        DecodeMode_Cpu,
        DecodeMode_Accelerator,
    };

    struct DspAdpcmLoopParam {
        u16 loopPredScale;
        u16 loopYn1;
        u16 loopYn2;
    };
    static_assert(sizeof(DspAdpcmLoopParam) == 0x6);

    struct OutputBusMixVolume {
        float volume[2][24];
    };
    static_assert(sizeof(OutputBusMixVolume) == 0xc0);

    struct OutputParam {
        float volume;
        u32 mixMode;
        MixParameter mixParameter[2];
        float pan;
        float span;
        float send[4];
    };
    static_assert(sizeof(OutputParam) == 0x50);

    struct SoundInstanceConfig {
        bool isBusMixVolumeEnabled;
        bool isVolumeThroughModeEnabled;
        s32 busCount;
    };
    static_assert(sizeof(SoundInstanceConfig) == 8);

    enum StreamFileType {
        StreamFileType_Bfstm,
        StreamFileType_Opus,
    };

    enum VoiceState {
        VoiceState_Play,
        VoiceState_Stop,
        VoiceState_Pause,
    };

    struct VoiceInfo {
        VoiceState voiceState;
        WaveBuffer::Status waveBufferStatus;
        void* waveBufferTag;
        u32 playPosition;
        void* userId;
    };
    static_assert(sizeof(VoiceInfo) == 0x20);

    class VoiceParam {
    private:
        float m_Volume;
        float m_Pitch;
        OutputMix m_TvMix;
        bool m_MonoFilterFlag;
        bool m_BiquadFilterFlag;
        BiquadFilterCoefficients m_BiquadFilterCoefficients;
        u16 m_MonoFilterCutoff;
        u8 m_InterpolationType;
    };
    static_assert(sizeof(VoiceParam) == 0x78);

    struct WaveInfo {
        struct ChannelParam {
            void* dataAddress;
            s32 dataSize;
            DspAdpcmParam adpcmParam;
            DspAdpcmLoopParam adpcmLoopParam;
        };
        static_assert(sizeof(ChannelParam) == 0x38);

        SampleFormat sampleFormat;
        bool loopFlag;
        s32 channelCount;
        s32 sampleRate;
        position_t loopStartFrame;
        position_t loopEndFrame;
        position_t originalLoopStartFrame;
        size_t dataSize;
        ChannelParam channelParam[2];
    };
    static_assert(sizeof(WaveInfo) == 0xa0);
}

// typedef -- SoundFrameUserCallback; 
// typedef -- SoundThreadUserCallback;
// typedef -- SoundStopCallback;



} // namespace nn::atk