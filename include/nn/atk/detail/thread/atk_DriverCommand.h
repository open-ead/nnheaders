#pragma once

#include <nn/atk/detail/atk_AdvancedWaveSoundPlayer.h>
#include <nn/atk/effect/atk_EffectAux.h>
#include <nn/atk/effect/atk_EffectBase.h>
#include <nn/atk/detail/seq/atk_SequenceSoundPlayer.h>
#include <nn/atk/detail/strm/atk_StreamSoundPlayer.h>
#include <nn/atk/detail/thread/atk_CommandManager.h>
#include <nn/atk/detail/wsd/atk_WaveSoundPlayer.h>

namespace nn::atk::detail {
enum DriverCommandId {
    DriverCommandId_Dummy,
    DriverCommandId_Debug,
    DriverCommandId_Reply,
    DriverCommandId_PlayerInit,
    DriverCommandId_PlayerPanmode,
    DriverCommandId_PlayerPancurve,
    DriverCommandId_PlayerFinalize,
    DriverCommandId_PlayerStart,
    DriverCommandId_PlayerStop,
    DriverCommandId_PlayerPause,
    DriverCommandId_PlayerParam,
    DriverCommandId_PlayerAdditionalSend,
    DriverCommandId_PlayerBusMixVolumeUsed,
    DriverCommandId_PlayerBusMixVolume,
    DriverCommandId_PlayerBusMixVolumeEnabled,
    DriverCommandId_PlayerBinaryVolume,
    DriverCommandId_PlayerVolumeThroughModeUsed,
    DriverCommandId_PlayerVolumeThroughMode,
    DriverCommandId_PlayerClearResourceFlag,
    DriverCommandId_SeqSetup,
    DriverCommandId_SeqLoad,
    DriverCommandId_SeqPrepare,
    DriverCommandId_SeqSkip,
    DriverCommandId_SeqTempoRatio,
    DriverCommandId_SeqChannelPrio,
    DriverCommandId_SeqSetVar,
    DriverCommandId_SeqSetGvar,
    DriverCommandId_SeqSetTvar,
    DriverCommandId_SeqTrackMute,
    DriverCommandId_SeqTrackSilence,
    DriverCommandId_SeqTrackVolume,
    DriverCommandId_SeqTrackPitch,
    DriverCommandId_SeqTrackLpf,
    DriverCommandId_SeqTrackBiquad,
    DriverCommandId_SeqTrackBankIndex,
    DriverCommandId_SeqTrackTranspose,
    DriverCommandId_SeqTrackVelocityRange,
    DriverCommandId_SeqTrackOutputline,
    DriverCommandId_SeqTrackOutputlineReset,
    DriverCommandId_SeqTrackTvVolume,
    DriverCommandId_SeqTrackTvMixParameter,
    DriverCommandId_SeqTrackTvPan,
    DriverCommandId_SeqTrackTvSpan,
    DriverCommandId_SeqTrackTvMainSend,
    DriverCommandId_SeqTrackTvFxSend,
    DriverCommandId_WsdPrepare,
    DriverCommandId_WsdLoad,
    DriverCommandId_WsdChannelPrio,
    DriverCommandId_WsdChannelParam,
    DriverCommandId_AwsdPrepare,
    DriverCommandId_StrmSetup,
    DriverCommandId_StrmPrepare,
    DriverCommandId_StrmPreparePrefetch,
    DriverCommandId_StrmLoadHeader,
    DriverCommandId_StrmLoadData,
    DriverCommandId_StrmForceFinish,
    DriverCommandId_StrmTrackVolume,
    DriverCommandId_StrmTrackInitialVolume,
    DriverCommandId_StrmTrackOutputline,
    DriverCommandId_StrmTrackOutputlineReset,
    DriverCommandId_StrmTrackTvVolume,
    DriverCommandId_StrmTrackTvMixParameter,
    DriverCommandId_StrmTrackTvPan,
    DriverCommandId_StrmTrackTvSpan,
    DriverCommandId_StrmTrackTvMainSend,
    DriverCommandId_StrmTrackTvFxSend,
    DriverCommandId_InvalidateData,
    DriverCommandId_RegistDisposeCallback,
    DriverCommandId_UnregistDisposeCallback,
    DriverCommandId_AppendEffect,
    DriverCommandId_AppendEffectAux,
    DriverCommandId_RemoveEffect,
    DriverCommandId_RemoveEffectAux,
    DriverCommandId_ClearEffect,
    DriverCommandId_SubMixApplyDestination,
    DriverCommandId_SubMixUpdateMixVolume,
    DriverCommandId_AuxBusVolume,
    DriverCommandId_AllVoicesSync,
    DriverCommandId_VoicePlay,
    DriverCommandId_VoiceWaveInfo,
    DriverCommandId_VoiceAdpcmParam,
    DriverCommandId_VoiceAppendWaveBuffer,
    DriverCommandId_VoicePriority,
    DriverCommandId_VoiceVolume,
    DriverCommandId_VoicePitch,
    DriverCommandId_VoicePanMode,
    DriverCommandId_VoicePanCurve,
    DriverCommandId_VoicePan,
    DriverCommandId_VoiceSpan,
    DriverCommandId_VoiceLpf,
    DriverCommandId_VoiceBiquad,
    DriverCommandId_VoiceOutputLine,
    DriverCommandId_VoiceMainOutVolume,
    DriverCommandId_VoiceMainSend,
    DriverCommandId_VoiceFxSend,
};

struct DriverCommandReply : Command {
    bool* ptr;
};
static_assert(sizeof(DriverCommandReply) == 0x20);

struct DriverCommandPlayerInit : Command {
    driver::BasicSoundPlayer* player;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* pOutputReceiver;
#endif
    bool* availableFlagPtr;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(DriverCommandPlayerInit) == 0x28);
#else
static_assert(sizeof(DriverCommandPlayerInit) == 0x30);
#endif

struct DriverCommandPlayerPanParam : Command {
    driver::BasicSoundPlayer* player;
    u8 panMode;
    u8 panCurve;
    u8 padding[2];
};
static_assert(sizeof(DriverCommandPlayerPanParam) == 0x28);

struct DriverCommandPlayer : Command {
    driver::BasicSoundPlayer* player;
    bool flag;
    u8 padding[3];
};
static_assert(sizeof(DriverCommandPlayer) == 0x28);

struct DriverCommandPlayerParam : Command {
    driver::BasicSoundPlayer* player;
    f32 volume;
    f32 pitch;
    f32 lpfFreq;
    s32 biquadFilterType;
    f32 biquadFilterValue;
    u32 outputLineFlag;
    OutputParam tvParam;
};
static_assert(sizeof(DriverCommandPlayerParam) == 0x88);

struct DriverCommandPlayerAdditionalSend : Command {
    driver::BasicSoundPlayer* player;
    s32 bus;
    f32 send;
};
static_assert(sizeof(DriverCommandPlayerAdditionalSend) == 0x28);

struct DriverCommandPlayerBusMixVolumeUsed : Command {
    driver::BasicSoundPlayer* player;
    bool isUsed;
};
static_assert(sizeof(DriverCommandPlayerBusMixVolumeUsed) == 0x28);

struct DriverCommandPlayerBusMixVolume : Command {
    driver::BasicSoundPlayer* player;
    OutputBusMixVolume tvBusMixVolume;
};
static_assert(sizeof(DriverCommandPlayerBusMixVolume) == 0xe0);

struct DriverCommandPlayerBusMixVolumeEnabled : Command {
    driver::BasicSoundPlayer* player;
    s32 bus;
    bool isEnabled;
};
static_assert(sizeof(DriverCommandPlayerBusMixVolumeEnabled) == 0x28);

struct DriverCommandPlayerBinaryVolume : Command {
    driver::BasicSoundPlayer* player;
    f32 volume;
};
static_assert(sizeof(DriverCommandPlayerBinaryVolume) == 0x28);

struct DriverCommandPlayerVolumeThroughModeUsed : Command {
    driver::BasicSoundPlayer* player;
    bool isVolumeThroughModeUsed;
};
static_assert(sizeof(DriverCommandPlayerVolumeThroughModeUsed) == 0x28);

struct DriverCommandPlayerVolumeThroughMode : Command {
    driver::BasicSoundPlayer* player;
    s32 bus;
    u8 volumeThroughMode;
};
static_assert(sizeof(DriverCommandPlayerVolumeThroughMode) == 0x28);

struct DriverCommandPlayerClearResourceFlag : Command {
    driver::BasicSoundPlayer* player;
};
static_assert(sizeof(DriverCommandPlayerClearResourceFlag) == 0x20);

struct DriverCommandSequenceSoundSetup : Command {
    driver::SequenceSoundPlayer* player;
    driver::SequenceSoundPlayer::SetupArg arg;
    u8 channelPriority;
    bool isReleasePriorityFix;
    std::uintptr_t userproc;
    void* userprocArg;
};
static_assert(sizeof(DriverCommandSequenceSoundSetup) == 0x50);

struct DriverCommandSequenceSoundLoad : Command {
    driver::SequenceSoundPlayer* player;
    driver::SequenceSoundPlayer::StartInfo startInfo;
    driver::SequenceSoundLoader::Arg arg;
};
static_assert(sizeof(DriverCommandSequenceSoundLoad) == 0xa0);

struct DriverCommandSequenceSoundPrepare : Command {
    driver::SequenceSoundPlayer* player;
    driver::SequenceSoundPlayer::PrepareArg arg;
};
static_assert(sizeof(DriverCommandSequenceSoundPrepare) == 0x80);

struct DriverCommandSequenceSoundSkip : Command {
    driver::SequenceSoundPlayer* player;
    s32 offsetType;
    s32 offset;
};
static_assert(sizeof(DriverCommandSequenceSoundSkip) == 0x28);

struct DriverCommandSequenceSoundTempoRatio : Command {
    driver::SequenceSoundPlayer* player;
    f32 tempoRatio;
};
static_assert(sizeof(DriverCommandSequenceSoundTempoRatio) == 0x28);

struct DriverCommandSequenceSoundChannelPrio : Command {
    driver::SequenceSoundPlayer* player;
    u8 priority;
    u8 padding[3];
};
static_assert(sizeof(DriverCommandSequenceSoundChannelPrio) == 0x28);

struct DriverCommandSequenceSoundSetVar : Command {
    driver::SequenceSoundPlayer* player;
    s32 trackNo;
    s32 varNo;
    s32 var;
};
static_assert(sizeof(DriverCommandSequenceSoundSetVar) == 0x30);

struct DriverCommandSequenceSoundTrack : Command {
    driver::SequenceSoundPlayer* player;
    u32 trackBitFlag;
};
static_assert(sizeof(DriverCommandSequenceSoundTrack) == 0x28);

struct DriverCommandSequenceSoundTrackMute : DriverCommandSequenceSoundTrack {
    s32 mute;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackMute) == 0x28);

struct DriverCommandSequenceSoundTrackSilence : DriverCommandSequenceSoundTrack {
    bool silenceFlag;
    s32 fadeFrames;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackSilence) == 0x30);

struct DriverCommandSequenceSoundTrackParam : DriverCommandSequenceSoundTrack {
    f32 value;
    u32 uint32Value;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackParam) == 0x30);

struct DriverCommandSequenceSoundTrackBiquad : DriverCommandSequenceSoundTrack {
    s32 type;
    f32 value;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackBiquad) == 0x30);

struct DriverCommandSequenceSoundTrackBankIndex : DriverCommandSequenceSoundTrack {
    s32 bankIndex;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackBankIndex) == 0x28);

struct DriverCommandSequenceSoundTrackTranspose : DriverCommandSequenceSoundTrack {
    s8 transpose;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackTranspose) == 0x28);

struct DriverCommandSequenceSoundTrackVelocityRange : DriverCommandSequenceSoundTrack {
    u8 range;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackVelocityRange) == 0x28);

struct DriverCommandSequenceSoundTrackOutputLine : DriverCommandSequenceSoundTrack {
    u32 outputLine;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackOutputLine) == 0x28);

struct DriverCommandSequenceSoundTrackMixParameter : DriverCommandSequenceSoundTrack {
    u32 trackBitFlag;
    u32 srcChNo;
    MixParameter param;
    u32 drcIndex;
};
static_assert(sizeof(DriverCommandSequenceSoundTrackMixParameter) == 0x48);

struct DriverCommandWaveSoundPrepare : Command {
    driver::WaveSoundPlayer* player;
    driver::WaveSoundPlayer::StartInfo startInfo;
    driver::WaveSoundPlayer::PrepareArg arg;
};
static_assert(sizeof(DriverCommandWaveSoundPrepare) == 0x60);

struct DriverCommandWaveSoundLoad : Command {
    driver::WaveSoundPlayer* player;
    driver::WaveSoundPlayer::StartInfo startInfo;
    driver::WaveSoundLoader::Arg arg;
};
static_assert(sizeof(DriverCommandWaveSoundLoad) == 0x78);

struct DriverCommandWaveSoundChannelPrio : Command {
    driver::WaveSoundPlayer* player;
    u8 priority;
    u8 padding[3];
};
static_assert(sizeof(DriverCommandWaveSoundChannelPrio) == 0x28);

struct DriverCommandWaveSoundChannelParam : Command {
    driver::WaveSoundPlayer* player;
    u8 priority;
    bool isReleasePriorityFix;
    u8 padding[2];
};
static_assert(sizeof(DriverCommandWaveSoundChannelParam) == 0x28);

struct DriverCommandAdvancedWaveSoundPrepare : Command {
    driver::AdvancedWaveSoundPlayer* player;
    driver::AdvancedWaveSoundPlayer::PrepareParameter parameter;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(DriverCommandAdvancedWaveSoundPrepare) == 0x40);
#else
static_assert(sizeof(DriverCommandAdvancedWaveSoundPrepare) == 0x38);
#endif

struct DriverCommandStreamSoundSetup : Command {
    driver::StreamSoundPlayer* player;
    driver::StreamSoundPlayer::SetupArg arg;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(DriverCommandStreamSoundSetup) == 0xb8);
#else
static_assert(sizeof(DriverCommandStreamSoundSetup) == 0xc0);
#endif
struct DriverCommandStreamSoundPrepare : Command {
    driver::StreamSoundPlayer* player;
    driver::StreamSoundPlayer::PrepareArg arg;
};
static_assert(sizeof(DriverCommandStreamSoundPrepare) == 0x300);

struct DriverCommandStreamSoundPreparePrefetch : Command {
    driver::StreamSoundPlayer* player;
    driver::StreamSoundPlayer::PreparePrefetchArg arg;
};
static_assert(sizeof(DriverCommandStreamSoundPreparePrefetch) == 0x2f8);

struct DriverCommandStreamSoundLoadHeader : Command {
    driver::StreamSoundPlayer* player;
    AdpcmParam* adpcmParam[16];
    bool result;
    u16 assignNumber;
};
static_assert(sizeof(DriverCommandStreamSoundLoadHeader) == 0xa8);

struct DriverCommandStreamSoundLoadData : Command {
    driver::StreamSoundPlayer* player;
    LoadDataParam loadDataParam;
    bool result;
    u16 assignNumber;
};
static_assert(sizeof(DriverCommandStreamSoundLoadData) == 0xc0);

struct DriverCommandStreamSoundForceFinish : Command {
    driver::StreamSoundPlayer* player; 
};
static_assert(sizeof(DriverCommandStreamSoundForceFinish) == 0x20);

struct DriverCommandStreamSoundTrackParam : Command {
    driver::StreamSoundPlayer* player;
    u32 trackBitFlag;
    f32 value;
    u32 uint32Value;
    u32 drcIndex;
};
static_assert(sizeof(DriverCommandStreamSoundTrackParam) == 0x30);

struct DriverCommandStreamSoundTrackInitialVolume : Command {
    driver::StreamSoundPlayer* player;
    u32 trackBitFlag;
    u32 value;
};
static_assert(sizeof(DriverCommandStreamSoundTrackInitialVolume) == 0x28);

struct DriverCommandStreamSoundTrackMixParameter : Command {
    driver::StreamSoundPlayer* player;
    u32 trackBitFlag;
    u32 srcChNo;
    MixParameter param;
    u32 drcIndex;
};
static_assert(sizeof(DriverCommandStreamSoundTrackMixParameter) == 0x48);

struct DriverCommandInvalidateData : Command {
    void* mem;
    size_t size;
};
static_assert(sizeof(DriverCommandInvalidateData) == 0x28);

struct DriverCommandDisposeCallback : Command {
    driver::DisposeCallback* callback;
};
static_assert(sizeof(DriverCommandDisposeCallback) == 0x20);

struct DriverCommandEffect : Command {
    s32 bus;
    EffectBase* effect;
    void* effectBuffer;
    size_t effectBufferSize;
    OutputMixer* pOutputMixer;
};
static_assert(sizeof(DriverCommandEffect) == 0x40);

struct DriverCommandEffectAux : Command {
    s32 bus;
    EffectAux* effect;
    void* effectBuffer;
    size_t effectBufferSize;
    OutputMixer* pOutputMixer;
};
static_assert(sizeof(DriverCommandEffectAux) == 0x40);

struct DriverCommandSubMixApplyDestination : Command {
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* pReceiver;
#endif
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(DriverCommandSubMixApplyDestination) == 0x18);
#else
static_assert(sizeof(DriverCommandSubMixApplyDestination) == 0x20);
#endif

struct DriverCommandSubMixUpdateMixVolume : Command {
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* pReceiver;
#endif
    s32 srcBus;
    s32 srcChannel;
    s32 dstBus;
    s32 dstChannel;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(DriverCommandSubMixUpdateMixVolume) == 0x28);
#else
static_assert(sizeof(DriverCommandSubMixUpdateMixVolume) == 0x30);
#endif

struct DriverCommandAuxBusVolume : Command {
    AuxBus bus;
    s32 subMixIndex;
    f32 volume;
    s32 fadeFrames;
};
static_assert(sizeof(DriverCommandAuxBusVolume) == 0x28);

struct DriverCommandAllVoicesSync : Command {
    u32 syncFlag;
};
static_assert(sizeof(DriverCommandAllVoicesSync) == 0x20);

struct DriverCommandVoice : Command {
    driver::MultiVoice* voice;
};
static_assert(sizeof(DriverCommandVoice) == 0x20);

struct DriverCommandVoicePlay : DriverCommandVoice {
    enum State {
        State_Start,
        State_Stop,
        State_PauseOn,
        State_PauseOff,
    };

    State state;
};
static_assert(sizeof(DriverCommandVoicePlay) == 0x28);

struct DriverCommandVoiceWaveInfo : DriverCommandVoice {
    SampleFormat format;
    s32 sampleRate;
    s32 interpolationType;
};
static_assert(sizeof(DriverCommandVoiceWaveInfo) == 0x30);

struct DriverCommandVoiceAdpcmParam : DriverCommandVoice {
    s32 channel;
    s32 voiceOut;
    AdpcmParam* param;
};
static_assert(sizeof(DriverCommandVoiceAdpcmParam) == 0x30);

class DriverCommand : CommandManager {
public:
    static void ProcessCommandList(Command* commandList);

    DriverCommand();

    void Initialize(void* commandBuffer, size_t commandBufferSize);

    void RequestProcessCommand();
};
static_assert(sizeof(DriverCommand) == 0x310);
} // namespace nn::atk::detail