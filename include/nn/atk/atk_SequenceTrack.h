#pragma once

#include <nn/atk/atk_Channel.h>
#include <nn/atk/atk_ProfileReader.h>
#include <nn/os/os_TickTypes.h>

namespace nn::atk::detail::driver {

class SequenceSoundPlayer;

class SequenceTrack {
public:
    static const int CallStackDepth{10};

    struct ParserTrackParam {
        struct CallStack {
            u8 loopFlag;
            u8 loopCount;
            u8 padding[2];
            const u8* address;
        };
        static_assert(sizeof(CallStack) == 0x10);

        const u8* baseAddr;
        const u8* currentAddr;
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
        int prgNo;
        float sweepPitch;
        MoveValue<u8, s16> volume;
        MoveValue<u8, s16> volume2;
        MoveValue<s8, s16> pan;
        MoveValue<s8, s16> surroundPan;
        MoveValue<s8, s16> pitchBend;
        CurveLfoParam lfoParam[Channel::ModCount]{};
        u8 lfoTarget[Channel::ModCount];
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
        u8 fxSend[AuxBus_Count];
        u8 padding2[1];
        float lpfFreq;
        float biquadValue;
        s32 outputLine;

        ParserTrackParam() = default;
    };
    static_assert(sizeof(ParserTrackParam) == 0x150);

    enum ParseResult {
        ParseResult_Continue,
        ParseResult_Finish,
    };

    static const int DefaultPriority{64};
    static const int DefaultBendRange{2};
    static const int DefaultPortaKey{60};
    static const int InvalidEnvelope{255};
    static const int MaxEnvelopeValue{127};
    static const int ParserParamSize{32};
    static const int TrackVariableCount{16};

    static const int PauseReleaseValue{127};
    static const int MuteReleaseValue{127};

    static void ChannelCallbackFunc(Channel* dropChannel, Channel::ChannelCallbackStatus,
                                    void* userData);

    SequenceTrack();
    virtual ~SequenceTrack();

    void InitParam();

    void SetSeqData(const void* seqBase, int seqOffset);

    void Open();
    void Close();

    bool IsOpened() const { return m_OpenFlag; }

    int ParseNextTick(bool doNoteOn);

    void UpdateChannelLength();
    void UpdateChannelParam();

    Channel* NoteOn(int key, int velocity, int length, bool tieFlag);

    void StopAllChannel();
    void ReleaseAllChannel(int release);
    void FreeAllChannel();
    void PauseAllChannel(bool flag);

    int GetChannelCount() const;

    const ParserTrackParam& GetParserTrackParam() const { return m_ParserTrackParam; }
    ParserTrackParam& GetParserTrackParam() { return m_ParserTrackParam; }

    void SetMute(SequenceMute mute);
    void SetSilence(bool silenceFlag, int fadeTimes);

    void SetVolume(float volume) { m_ExtVolume = volume; }
    void SetPitch(float pitch) { m_ExtPitch = pitch; }
    void SetPanRange(float panRange) { m_PanRange = panRange; }

    void SetLpfFreq(float lpfFreq) { m_ParserTrackParam.lpfFreq = lpfFreq; };
    void SetBiquadFilter(int type, float value);
    void SetBankIndex(int bankIndex);
    void SetTranspose(s8 transpose);
    void SetVelocityRange(u8 range);
    void SetOutputLine(int outputLine);

    void SetTvVolume(float volume) { m_TvParam.volume = volume; };
    void SetTvMixParameter(u32 srcChNo, int mixChNo, float param);
    void SetTvPan(float pan) { m_TvParam.pan = pan; }
    void SetTvSurroundPan(float span) { m_TvParam.span = span; }
    void SetTvMainSend(float mainSend) { m_TvParam.send[0] = mainSend; }
    void SetTvFxSend(AuxBus bus, float send) { m_TvParam.send[bus] = send; };

    float GetVolume() const { return m_ExtVolume; }
    float GetPitch() const { return m_ExtPitch; }
    float GetPanRange() const { return m_PanRange; }

    float GetLpfFreq() const { return m_ParserTrackParam.lpfFreq; }
    int GetBiquadType() const { return m_ParserTrackParam.biquadType; }
    float GetBiquadValue() const { return m_ParserTrackParam.biquadValue; }

    s16 GetTrackVariable(int varNo) const;
    void SetTrackVariable(int varNo, s16 var);
    volatile s16* GetVariablePtr(int varNo);

    void SetSequenceSoundPlayer(SequenceSoundPlayer* player) { m_pSequenceSoundPlayer = player; }
    const SequenceSoundPlayer* GetSequenceSoundPlayer() const { return m_pSequenceSoundPlayer; }
    SequenceSoundPlayer* GetSequenceSoundPlayer() { return m_pSequenceSoundPlayer; }

    void SetPlayerTrackNo(int playerTrackNo);
    u8 GetPlayerTrackNo() const { return m_PlayerTrackNo; }

    void UpdateChannelRelease(Channel* channel);

    os::Tick GetProcessTick(const SoundProfile& profile);

    void ForceMute();

protected:
    virtual ParseResult Parse(bool doNoteOn) = 0;

private:
    Channel* GetLastChannel() const { return m_pChannelList; };

    void AddChannel(Channel* channel);

    u8 m_PlayerTrackNo;
    bool m_OpenFlag;
    bool m_ForceMute;
    float m_ExtVolume;
    float m_ExtPitch;
    float m_PanRange;
    OutputParam m_TvParam;
    ParserTrackParam m_ParserTrackParam;
    volatile s16 m_TrackVariable[TrackVariableCount];
    SequenceSoundPlayer* m_pSequenceSoundPlayer;
    Channel* m_pChannelList;
};
static_assert(sizeof(SequenceTrack) == 0x1e8);

}  // namespace nn::atk::detail::driver