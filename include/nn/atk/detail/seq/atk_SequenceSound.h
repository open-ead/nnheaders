#pragma once

#include <nn/atk/atk_SequenceSoundHandle.h>
#include <nn/atk/detail/atk_SoundInstanceManager.h>
#include <nn/atk/detail/seq/atk_SequenceSoundPlayer.h>

namespace nn::atk::detail {
class SequenceSound;
using SequenceSoundInstanceManager = SoundInstanceManager<SequenceSound>;

class SequenceSound : BasicSound {
public:
    constexpr static u32 BankIndexMin = 0;
    constexpr static u32 BankIndexMax = 3;

    constexpr static u8 TransposeMin = 192;
    constexpr static u8 TransposeMax = 63;

    constexpr static u8 VelocityRangeMin = 0;
    constexpr static u32 VelocityRangeMax = 0x7f00;

    struct Resource {
        void* seq;
        void* banks[4];
        void* warcs[4];
        bool warcIsIndividuals[4];
    };
    static_assert(sizeof(Resource) == 0x50);

    explicit SequenceSound(SequenceSoundInstanceManager& manager);
    ~SequenceSound() override;

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    bool Initialize() override;
#else
    bool Initialize(OutputReceiver* pOutputReceiver) override;
#endif
    void Finalize() override;

    void Setup(driver::SequenceTrackAllocator* trackAllocator, u32 allocTracks, 
               driver::NoteOnCallback* noteOnCallback, s32 channelPriority, 
               bool isReleasePriorityFix, SequenceUserProcCallback userproc, void* userprocArg);

    void Prepare(const Resource& res, const driver::SequenceSoundPlayer::StartInfo& startInfo);

    void Skip(driver::SequenceSoundPlayer::StartOffsetType, s32);

    void SetTempoRatio(f32 tempoRatio);
    
    void SetChannelPriority(s32 priority);
    void OnUpdatePlayerPriority() override;

    void SetTrackMute(u32 trackBitFlag, SequenceMute mute);
    void SetTrackMute(u32 trackBitFlag, bool);
    void SetTrackSilence(u32 trackBitFlag, bool, s32);
    void SetTrackVolume(u32 trackBitFlag, f32 volume);
    void SetTrackPitch(u32 trackBitFlag, f32 pitch);
    void SetTrackMainOutVolume(u32 trackBitFlag, f32 volume);
    void SetTrackChannelMixParameter(u32 trackBitFlag, u32 srcChNo, const MixParameter& param);
    void SetTrackPan(u32 trackBitFlag, f32 pan);
    void SetTrackSurroundPan(u32 trackBitFlag, f32 surroundPan);
    void SetTrackMainSend(u32 trackBitFlag, f32 send);
    void SetTrackFxSend(u32 trackBitFlag, AuxBus bus, f32 send);
    void SetTrackLpfFreq(u32 trackBitFlag, f32 lpfFreq);
    void SetTrackBiquadFilter(u32 trackBitFlag, s32 type, f32 value);
    void SetTrackBankIndex(u32 trackBitFlag, s32 bankIndex);
    void SetTrackTranspose(u32 trackBitFlag, s8 transpose);
    void SetTrackVelocityRange(u32 trackBitFlag, u8 range);

    void SetTrackOutputLine(u32 trackBitFlag, u32 outputLine);
    void ResetTrackOutputLine(u32 trackBitFlag);

    bool ReadVariable(s32 varNo, s16* varPtr) const;
    static bool ReadGlobalVariable(s32 varNo, s16* varPtr);
    bool ReadTrackVariable(s32, s32, s16*) const;

    void WriteVariable(s32 varNo, s16 var);
    static void WriteGlobalVariable(s32 varNo, s16 var);
    void WriteTrackVariable(s32, s32, s16 var);

    u64 GetTick() const;

    bool IsAttachedTempSpecialHandle() override;
    void DetachTempSpecialHandle() override;

    void RegisterDataLoadTask(const driver::SequenceSoundLoader::LoadInfo& loadInfo, 
                              const driver::SequenceSoundPlayer::StartInfo& startInfo);

    bool IsPrepared() const override;

    driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() override;

    void OnUpdateParam() override;


private:
    friend SequenceSoundInstanceManager;

    util::IntrusiveListNode m_PriorityLink;
    SequenceSoundHandle* m_pTempSpecialHandle;
    SequenceSoundInstanceManager* m_Manager;
    bool m_InitializeFlag;
    bool m_CanUseTask;
    bool m_IsCalledPrepare;
    u8 m_Padding[1];
    driver::SequenceSoundPlayer m_PlayerInstance;
};
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(SequenceSound) == 0x5a0);
#else
static_assert(sizeof(SequenceSound) == 0x578);
#endif
} // namespace nn::atk::detail