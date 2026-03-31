#pragma once

#include <nn/atk/detail/voice/atk_Channel.h>

namespace nn::atk::detail::driver {
class SequenceSoundPlayer;

class SequenceTrack {
public:
    enum ParseResult {
        ParseResult_Continue,
        ParseResult_Finish,
    };

    constexpr static u32 CallStackDepth = 10;

    constexpr static u32 DefaultPriority = 64;
    constexpr static u32 DefaultBendRange = 2;
    constexpr static u32 DefaultPortaKey = 60;

    constexpr static s32 InvalidEnvelope = 255;
    constexpr static s32 MaxEnvelopeValue = 127;

    constexpr static s32 ParserParamSize = 32;

    constexpr static s32 TrackVariableCount = 16;

    constexpr static s32 PauseReleaseValue = 16;
    constexpr static s32 MuteReleaseValue = 127;


    struct ParserTrackParam {
        struct CallStack {
            u8 loopFlag;
            u8 loopCount;
            u8 padding[2];
            u8* address;
        };
        static_assert(sizeof(CallStack) == 0x10);

        u8* baseAddr;
        u8* currentAddr;
        bool cmpFlag;
        bool noteWaitFlag;
        bool tieFlag;
        bool monophonicFlag;
        CallStack callStack[CallStackDepth];
        u8 callStackDepth;
        bool frontBypassFlag;
        bool muteFlag;
        bool silenceFlag;
        s32 wait;
        bool noteFinishWait;
        bool portaFlag;
        bool damperFlag;
        u8 bankIndex;
        s32 prgNo;
        f32 sweepPitch;
        MoveValue<u8, s16> volume;
        MoveValue<u8, s16> volume2;
        MoveValue<s8, s16> pan;
        MoveValue<s8, s16> surroundPan;
        MoveValue<s8, s16> pitchBend;
        CurveLfoParam lfoParam[4];
        u8 lfoTarget[4];
        u8 velocityRange;
        u8 bendRange;
        s8 initPan;
        u8 padding1[1];
        s8 transpose;
        u8 priority;
        u8 portaKey;
        u8 portaTime;
        u8 attack;
        u8 decay;
        u8 sustain;
        u8 release;
        s16 envHold;
        s8 biquadType;
        u8 mainSend;
        u8 fxSend[3];
        u8 padding2[1];
        f32 lpfFreq;
        f32 biquadValue;
        s32 outputLine; 
    };
    static_assert(sizeof(ParserTrackParam) ==  0x150);

    virtual ~SequenceTrack();
    virtual ParseResult Parse(bool doNoteOn) = 0;

    SequenceTrack();

    void SetPlayerTrackNo(s32 playerTrackNo);

    void InitParam();

    void Close();

    void SetSeqData(const void* seqBase, s32 seqOffset);

    void Open();

    void ReleaseAllChannel(s32 release);
    void FreeAllChannel();

    void UpdateChannelLength();
    void UpdateChannelRelease(Channel*);

    s32 ParseNextTick(bool doNoteOn);

    void StopAllChannel();
    void UpdateChannelParam();
    void PauseAllChannel(bool flag);
    
    void AddChannel(Channel*);

    s32 GetChannelCount() const;

    static void ChannelCallbackFunc(Channel* dropChannel, 
                                    Channel::ChannelCallbackStatus callbackStatus, void* userData);

    void SetMute(SequenceMute mute);
    void ForceMute();

    void SetSilence(bool silenceFlag, s32 fadeTimes);
    void SetBiquadFilter(s32 type, f32 value);
    void SetBankIndex(s32 bankIndex);
    void SetTranspose(s8 transpose);
    void SetVelocityRange(u8 range);
    void SetOutputLine(s32 outputLine);
    void SetTvMixParameter(u32 srcChNo, s32 mixChNo, f32 param);
    
    s16 GetTrackVariable(s32 varNo) const;
    void SetTrackVariable(s32 varNo, s16 var);

    s16* GetVariablePtr(s32 varNo);

    Channel* NoteOn(s32 key, s32 velocity, s32 length, bool tieFlag);

private:
    u8 m_PlayerTrackNo;
    bool m_OpenFlag;
    bool m_ForceMute;
    f32 m_ExtVolume;
    f32 m_ExtPitch;
    f32 m_PanRange;
    OutputParam m_TvParam;
    ParserTrackParam m_ParserTrackParam;
    s16 m_TrackVariable[TrackVariableCount];
    SequenceSoundPlayer* m_pSequenceSoundPlayer;
    Channel* m_pChannelList;
};
static_assert(sizeof(SequenceTrack) == 0x1e8);
} // namespace nn::atk::detail::driver