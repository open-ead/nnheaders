#pragma once

#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/detail/atk_BasicSoundPlayer.h>
#include <nn/atk/detail/atk_LoaderManager.h>
#include <nn/atk/detail/atk_NoteOnCallback.h>
#include <nn/atk/detail/atk_PlayerHeap.h>
#include <nn/atk/detail/atk_WaveArchiveFileReader.h>
#include <nn/atk/detail/seq/atk_BankFileReader.h>
#include <nn/atk/detail/seq/atk_SequenceTrackAllocator.h>
#include <nn/atk/detail/thread/atk_Task.h>

namespace nn::atk {
struct SequenceUserProcCallbackParam {
    s16* localVariable;
    s16* globalVariable;
    s16* trackVariable;
    bool cmpFlag;
};
static_assert(sizeof(SequenceUserProcCallbackParam) == 0x20);

using SequenceUserProcCallback = void(*)(u16,SequenceUserProcCallbackParam*,void*);

namespace detail::driver {
class SequenceSoundLoader {
public:
    struct LoadInfo {
        LoadInfo(SoundArchive* arc, SoundDataManager* mgr, 
                 LoadItemInfo* seq, LoadItemInfo* banks, SoundPlayer* player);

        SoundArchive* soundArchive;
        SoundDataManager* soundDataManager;
        LoadItemInfo* loadInfoSeq;
        LoadItemInfo* loadInfoBanks[4];
        SoundPlayer* soundPlayer;
    };
    static_assert(sizeof(LoadInfo) == 0x40);

    struct Arg {
        SoundArchive* soundArchive;
        SoundDataManager* soundDataManager;
        SoundPlayer* soundPlayer;
        LoadItemInfo loadInfoSeq;
        LoadItemInfo loadInfoBanks[4];
    };
    static_assert(sizeof(Arg) == 0x68);

    struct Data {
        void* seqFile;
        void* bankFiles[4];
        void* warcFiles[4];
        bool warcIsIndividuals[4];
    };
    static_assert(sizeof(Data) == 0x50);

    class DataLoadTask : Task {
    public:
        DataLoadTask();
        ~DataLoadTask() override;

        bool TryAllocPlayerHeap();

        void Execute(TaskProfileLogger& logger) override;

    private:
        Arg m_Arg;
        Data m_Data;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;
        bool m_IsLoadSuccess;
        u8 m_Padding[3];
    };
    static_assert(sizeof(DataLoadTask) == 0x118);

    class FreePlayerHeapTask : Task {
    public:
        FreePlayerHeapTask();
        ~FreePlayerHeapTask() override;

        void Initialize();
        void Execute(TaskProfileLogger& logger) override;

    private:
        Arg m_Arg;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;

    };

    ~SequenceSoundLoader();

    void Initialize(const Arg& arg);

    void Finalize();

    bool IsInUse();

    bool TryWait();

private:
    friend LoaderManager<SequenceSoundLoader>;

    DataLoadTask m_Task;
    FreePlayerHeapTask m_FreePlayerHeapTask;
    PlayerHeapDataManager m_PlayerHeapDataManager;
    util::IntrusiveListNode m_LinkForLoaderManager;
};
static_assert(sizeof(SequenceSoundLoader) == 0x4b0);

using SequenceSoundLoaderManager = LoaderManager<SequenceSoundLoader>;

class SequenceSoundPlayer : BasicSoundPlayer, DisposeCallback, SoundThread::PlayerCallback {
public:
    enum StartOffsetType {
        StartOffsetType_Tick,
        StartOffsetType_Millisec,
    };

    enum ResState {
        ResState_Invalid,
        ResState_RecvLoadReq,
        ResState_AppendLoadTask,
        ResState_Assigned,
    };

    constexpr static s32 PlayerVariableCount = 16;
    constexpr static s32 GlobalVariableCount = 16;
    constexpr static s32 TrackCountPerPlayer = 16;
    
    constexpr static u32 AllTrackBitFlag = 0x0000FFFF;

    constexpr static s32 VariableDefaultValue = -1;
    constexpr static s32 DefaultTimebase = 48;
    constexpr static s32 DefaultTempo = 120;
    constexpr static u64 DefaultSkipIntervalTick = 0x300;

    struct ParserPlayerParam {
        u8 priority;
        u8 timebase;
        u16 tempo;
        MoveValue<u8, s16> volume;
        NoteOnCallback* callback;
    };
    static_assert(sizeof(ParserPlayerParam) == 0x18);

    struct StartInfo {
        s32 seqOffset;
        StartOffsetType startOffsetType;
        s32 startOffset;
        s32 delayTime;
        s32 delayCount;
        UpdateType updateType;
    };
    static_assert(sizeof(StartInfo) == 0x18);
    
    struct PrepareArg {
        void* seqFile;
        void* bankFiles[4];
        void* warcFiles[4];
        bool warcIsIndividuals[4];
        s32 seqOffset;
        s32 delayTime;
        s32 delayCount;
        UpdateType updateType;
    };
    static_assert(sizeof(PrepareArg) == 0x60);

    struct SetupArg {
        SequenceTrackAllocator* trackAllocator;
        u32 allocTracks;
        NoteOnCallback* callback;
    };
    static_assert(sizeof(SetupArg) == 0x18);

    static void InitSequenceSoundPlayer();

    SequenceSoundPlayer();
    ~SequenceSoundPlayer() override;

    void Initialize();
    void Initialize(OutputReceiver* pOutputReceiver) override;
    void Finalize() override;

    void FinishPlayer();

    void FreeLoader();

    void Setup(const SetupArg& arg);

    void SetPlayerTrack(s32 trackNo, SequenceTrack* track);
    SequenceTrack* GetPlayerTrack(s32 trackNo);

    void Start() override;
    void Stop() override;
    void Pause(bool flag) override;
    void Skip(StartOffsetType offsetType, s32 offset);

    void SetTempoRatio(f32 tempoRatio);
    void SetPanRange(f32 panRange);
    void SetChannelPriority(s32 priority);
    void SetReleasePriorityFix(bool fix);
    void SetSequenceUserprocCallback(SequenceUserProcCallback callback, void* arg);
    
    void CallSequenceUserprocCallback(u16 procId, SequenceTrack* track);

    s16* GetVariablePtr(s32 varNo);
    void GetLocalVariable(s32 varNo) const;
    void SetLocalVariable(s32 varNo, s16 var);
    static void SetGlobalVariable(s32 varNo, s16 var);

    void SetTrackMute(u32 trackBitFlag, SequenceMute mute);
    void SetTrackSilence(u64 trackBitFlag, bool silenceFlag, s32 fadeTimes);
    void SetTrackVolume(u32 trackBitFlag, f32 volume);
    void SetTrackPitch(u32 trackBitFlag, f32 pitch);
    void SetTrackLpfFreq(u32 trackBitFlag, f32 lpfFreq);
    void SetTrackBiquadFilter(u32 trackBitFlag, s32 type, f32 value);
    bool SetTrackBankIndex(u32 trackBitFlag, s32 bankIndex);
    void SetTrackTranspose(u32 trackBitFlag, s8 transpose);
    void SetTrackVelocityRange(u32 trackBitFlag, u8 range);
    
    void SetTrackOutputLine(u32 trackBitFlag, u32 outputLine);
    void ResetTrackOutputLine(u32 trackBitFlag);

    void SetTrackTvVolume(u32 trackBitFlag, f32 volume);
    void SetTrackChannelTvMixParameter(u32 trackBitFlag, u32 srcChNo, const MixParameter& param);
    void SetTrackTvPan(u32 trackBitFlag, f32 pan);
    void SetTrackTvSurroundPan(u32 trackBitFlag, f32 surroundPan);
    void SetTrackTvMainSend(u32 trackBitFlag, f32 send);
    void SetTrackTvFxSend(u32 trackBitFlag, AuxBus bus, f32 send);

    void InvalidateData(const void* start, const void* end) override;

    SequenceTrack* GetPlayerTrack(s32 trackNo) const;
    void CloseTrack(s32 trackNo);

    void UpdateChannelParam();

    s32 ParseNextTick(bool doNoteOn);

    void Update();

    void PrepareForPlayerHeap(PrepareArg* arg);

    void SkipTick();
    void UpdateTick();

    Channel* NoteOn(u8 bankIndex, NoteOnInfo* noteOnInfo);

    void Prepare(PrepareArg* arg);

    void RequestLoad(StartInfo* info, SequenceSoundLoader::Arg* arg);

    bool TryAllocLoader();

private:
    bool m_ReleasePriorityFixFlag;
    bool m_IsPrepared;
    f32 m_PanRange;
    f32 m_TempoRatio;
    f32 m_TickFraction;
    u32 m_SkipTickCounter;
    f32 m_SkipTimeCounter;
    s32 m_DelayCount;
    ParserPlayerParam m_ParserParam;
    SequenceTrackAllocator* m_pSequenceTrackAllocator;
    SequenceUserProcCallback m_SequenceUserprocCallback;
    void* m_pSequenceUserprocCallbackArg;
    SequenceTrack* m_pTracks[TrackCountPerPlayer];
    s16 m_LocalVariable[PlayerVariableCount];
    u32 m_TickCounter;
    WaveArchiveFileReader m_WarcFileReader[4];
    BankFileReader m_BankFileReader[4];
    u8 m_ResState;
    bool m_IsInitialized;
    bool m_IsRegisterPlayerCallback;
    u8 m_Padding[1];
    StartInfo m_StartInfo;
    SequenceSoundLoaderManager* m_pLoaderManager;
    SequenceSoundLoader* m_pLoader;
    SequenceSoundLoader::Arg m_LoaderArg;
    UpdateType m_UpdateType;

    static s16 m_GlobalVariable[GlobalVariableCount];
    static s32 m_SkipIntervalTickPerFrame;
};
static_assert(sizeof(SequenceSoundPlayer) == 0x368);
} // namespace nn::atk::detail::driver
} // namespace nn::atk