#pragma once

#include <atomic>

#include <nn/audio/audio_MemoryPoolTypes.h>
#include <nn/audio/audio_SinkTypes.h>

#include <nn/atk/atk_BiquadFilterCallback.h>
#include <nn/atk/detail/voice/atk_LowLevelVoice.h>
#include <nn/atk/submix/atk_FinalMix.h>
#include <nn/atk/submix/atk_SubMix.h>
#include <nn/atk/util/atk_DeviceOutRecorder.h>

namespace nn::atk::detail::driver {
class HardwareManager { // inherits nn::atk::Util::Singleton<HardwareManager>
public:
    using SubMixList = util::IntrusiveList<SubMix, util::IntrusiveListMemberNodeTraits<SubMix, &SubMix::m_Link>>;

    constexpr static u32 SoundFrameIntervalMsec = 5;
    constexpr static u32 SoundFrameIntervalUsec = 5000;

    constexpr static u32 DefaultRendererSampleRate = 48000;
    constexpr static u32 DefaultRendererUserEffectCount = 10;
    constexpr static u32 DefaultRendererVoiceCountMax = 96;

    constexpr static u32 DefaultRecordingAudioFrameCount = 8;

    constexpr static u32 AtkVoiceCountMax = 192;
    constexpr static u32 MixerCount = 3;
    constexpr static u32 ChannelCountMax = 6;
    constexpr static u32 BusCount = 4;

    constexpr static u32 DefaultRendererSampleCount = 140;
    constexpr static u32 DefaultRendererMixBufferCount = 30;
    constexpr static u32 DefaultRendererSubMixCount = 1;
    constexpr static u32 DefaultRendererSinkCount = 1;
    constexpr static u32 DefaultRendererPerformanceFrameCount = 0;

    constexpr static u32 DefaultRendererSystemEffectCount = 4;

    constexpr static u32 SubMixCountMax = 2;
    constexpr static u32 SubMixCountForAdditionalEffect = 1;
    constexpr static u32 ChannelCountForAdditionalEffect = 2;

    constexpr static u32 AuxBusCountForAdditionalEffect = 2;
    constexpr static u32 MixBufferCountForAdditionalEffect = 6;

    constexpr static bool DefaultRendererIsVoiceDropEnabled = false;

    class EffectAuxListScopedLock {
    public:
        EffectAuxListScopedLock();
        ~EffectAuxListScopedLock();
    };
    void LockEffectAuxList();
    void UnlockEffectAuxList();

    class EffectAuxListForFinalMixScopedLock {
    public:
        EffectAuxListForFinalMixScopedLock();
        ~EffectAuxListForFinalMixScopedLock();
    };
    void LockEffectAuxListForFinalMix();
    void UnlockEffectAuxListForFinalMix();

    class EffectAuxListForAdditionalSubMixScopedLock {
    public:
        EffectAuxListForAdditionalSubMixScopedLock();
        ~EffectAuxListForAdditionalSubMixScopedLock();
    };
    void LockEffectAuxListForAdditionalSubMix();
    void UnlockEffectAuxListForAdditionalSubMix();

    class SubMixListScopedLock {
    public:
        SubMixListScopedLock();
        ~SubMixListScopedLock();
    };
    void LockSubMixList();
    void UnlockSubMixList();

    class UpdateAudioRendererScopedLock {
    public:
        UpdateAudioRendererScopedLock();
        ~UpdateAudioRendererScopedLock();
    };

    class HardwareManagerParameter {
    public:
        void SetSubMixParameter(bool isStereoModeEnabled, bool isEffectEnabled, 
                                bool isSubMixEnabled, bool isAdditionalEffectBusEnabled,
                                bool isAdditionalSubMixEnabled, bool isCustomSubMixEnabled, 
                                s32 customSubMixCount, s32 customMixTotalChannelCount);

    private:
        s32 m_RendererSampleRate;
        s32 m_UserEffectCount;
        s32 m_VoiceCount;
        s32 m_RecordingAudioFrameCount;
        s32 m_SubMixCount;
        s32 m_MixBufferCount;
        bool m_IsProfilerEnabled;
        bool m_IsAdditionalEffectBusEnabled;
        bool m_IsAdditionalSubMixEnabled;
        bool m_IsEffectEnabled;
        bool m_IsRecordingEnabled;
        bool m_IsUserCircularBufferSinkEnabled;
        bool m_IsPresetSubMixEnabled;
        bool m_IsStereoModeEnabled;
        bool m_IsSoundThreadEnabled;
        bool m_IsVoiceDropEnabled;
        bool m_IsCompatibleDownMixSettingEnabled;
        bool m_IsPreviousSdkVersionLowPassFilterCompatible;
        bool m_IsUnusedEffectChannelMutingEnabled;
        bool m_IsCompatibleBusVolumeEnabled;
        bool m_IsUserThreadRenderingEnabled;
        bool m_IsCustomSubMixEnabled;
        bool m_IsMemoryPoolAttachCheckEnabled;
    };
    static_assert(sizeof(HardwareManagerParameter) == 0x2c);

    HardwareManager();

    void ResetParameters();

    audio::MemoryPoolState GetMemoryPoolState(audio::MemoryPoolType* pPool);

    void SetupAudioRendererParameter(audio::AudioRendererParameter* audioRendererParameter, 
                                     const HardwareManagerParameter& hardwareManagerParameter) const;

    size_t GetRequiredMemSize(const HardwareManagerParameter& hardwareManagerParameter) const;
    size_t GetRequiredMemSizeForMemoryPool(s32 voiceCount) const;
    size_t GetRequiredRecorderWorkBufferSize(const HardwareManagerParameter& hardwareManagerParameter) const;
    size_t GetRequiredCircularBufferSinkWithMemoryPoolBufferSize(const HardwareManagerParameter& hardwareManagerParameter) const;
    size_t GetRequiredCircularBufferSinkBufferSize(const HardwareManagerParameter& hardwareManagerParameter) const;

    s32 GetChannelCountMax() const;

    bool RegisterRecorder(DeviceOutRecorder* pRecorder);
    bool UnregisterRecorder(DeviceOutRecorder* pRecorder);
    void UpdateRecorder();

    u64 ReadRecordingCircularBufferSink(void* buffer, size_t bufferSize);
    audio::CircularBufferSinkType* AllocateRecordingCircularBufferSink();
    void FreeRecordingCircularBufferSink(audio::CircularBufferSinkType* circularBufferSink);
    void StartRecordingCircularBufferSink();

    void StopUserCircularBufferSink();
    void StartUserCircularBufferSink(bool isForceStartMode);
    u64 ReadUserCircularBufferSink(void* buffer, size_t bufferSize);

    void AttachMemoryPool(audio::MemoryPoolType* pPool, void* buffer, 
                          size_t bufferSize, bool isSoundThreadEnabled);

    Result RequestUpdateAudioRenderer();

    void DetachMemoryPool(audio::MemoryPoolType* pPool, bool isSoundThreadEnabled);

    void ExecuteAudioRendererRendering();

    void WaitAudioRendererEvent();

    s32* GetDroppedLowLevelVoiceCount() const;

    size_t GetRequiredPerformanceFramesBufferSize(HardwareManagerParameter* hardwareManagerParameter);

    Result Initialize(void* buffer, size_t bufferSize, void* memoryPoolBuffer, size_t memoryPoolBufferSize,
                      void* circularBufferSinkBuffer, size_t circularBufferSinkBufferSize, 
                      HardwareManagerParameter* parameter);

    void SetBiquadFilterCallback(s32, const BiquadFilterCallback* callback);
    void SetOutputMode(OutputMode mode,OutputDevice device);

    void UpdateEndUserOutputMode();

    void Finalize();

    void Update();
    void UpdateEffect();

    void SuspendAudioRenderer();
    void ResumeAudioRenderer();

    bool TimedWaitAudioRendererEvent(TimeSpan timeout);

    void SetAudioRendererRenderingTimeLimit(s32 timeLimit);
    s32 GetAudioRendererRenderingTimeLimit();

    void PrepareReset();

    bool IsResetReady() const;

    void AddSubMix(SubMix* pSubMix);
    void RemoveSubMix(SubMix* pSubMix);

    SubMix* GetSubMix(s32 subMixNumber);
    SubMix* GetSubMix(s32 subMixNumber) const;

    s32 GetSubMixCount() const;
    s32 GetChannelCount() const;
    f32 GetOutputVolume() const;

    void SetOutputDeviceFlag(s32, u8);

    void SetMasterVolume(f32 volume, s32 fadeFrames);
    void SetSrcType(SampleRateConverterType sampleRateConverter);

    size_t GetRequiredEffectAuxBufferSize(const EffectAux* pEffect) const;

    void SetAuxBusVolume(AuxBus bus, f32 volume, s32 fadeFrames, s32 subMixIndex);
    f32 GetAuxBusVolume(AuxBus bus, s32 subMixIndex) const;

    void SetMainBusChannelVolumeForAdditionalEffect(f32 volume, s32 srcChannel, s32 dstChannel);
    f32 GetMainBusChannelVolumeForAdditionalEffect(s32 srcChannel, s32 dstChannel) const;

    void SetAuxBusChannelVolumeForAdditionalEffect(AuxBus bus, f32 volume, s32 srcChannel, s32 dstChannel);
    f32 GetAuxBusChannelVolumeForAdditionalEffect(AuxBus bus, s32 srcChannel, s32 dstChannel) const;

    static void FlushDataCache(void* address, size_t length);

private:
    bool m_IsInitialized;
    audio::AudioRendererHandle m_RendererHandle;
    audio::AudioRendererConfig m_Config;
    os::SystemEvent m_SystemEvent;
    s32 m_AudioRendererSuspendCount;
    std::atomic_ulong m_AudioRendererUpdateCount;
    void* m_pAudioRendererWorkBuffer;
    void* m_pAudioRendererConfigWorkBuffer;
    OutputMode m_OutputMode[1];
    OutputMode m_EndUserOutputMode[1];
    SampleRateConverterType m_SrcType;
    MoveValue<f32, s32> m_MasterVolume;
    MoveValue<f32, s32> m_VolumeForReset;
    BiquadFilterCallback* m_BiquadFilterCallbackTable[128];
    u8 m_OutputDeviceFlag[32];
    LowLevelVoiceAllocator m_LowLevelVoiceAllocator;
    FinalMix m_FinalMix;
    SubMix m_SubMix[SubMixCountMax];
    SubMix m_AdditionalSubMix;
    SubMixList m_SubMixList;
    fnd::CriticalSection m_SubMixListLock;
    audio::AudioRendererParameter m_AudioRendererParameter;
    audio::DeviceSinkType m_Sink;
    MoveValue<f32, s32> m_AuxUserVolume[MixerCount];
    MoveValue<f32, s32> m_AuxUserVolumeForAdditionalEffect[AuxBusCountForAdditionalEffect];
    bool m_IsInitializedEffect;
    bool m_IsPresetSubMixEnabled;
    bool m_IsAdditionalEffectEnabled;
    bool m_IsAdditionalSubMixEnabled;
    bool m_IsStereoModeEnabled;
    bool m_IsInitializedSoundThread;
    bool m_IsPreviousSdkVersionLowPassFilterCompatible;
    bool m_IsMemoryPoolAttachCheckEnabled;
    fnd::CriticalSection m_UpdateAudioRendererLock;
    fnd::CriticalSection m_UpdateHardwareManagerLock;
    fnd::CriticalSection m_EffectAuxListLock;
    fnd::CriticalSection m_EffectAuxListForFinalMixLock;
    fnd::CriticalSection m_EffectAuxListForAdditionalSubMixLock;
    audio::CircularBufferSinkType m_RecordingCircularBufferSink;
    CircularBufferSinkState m_RecordingCircularBufferSinkState;
    audio::MemoryPoolType m_RecordingCircularBufferSinkMemoryPool;
    void* m_RecordingCircularBufferSinkBuffer;
    bool m_IsRecordingCircularBufferSinkAllocated;
    void* m_RecordingBuffer;
    size_t m_RecordingBufferSize;
    DeviceOutRecorder* m_pRecorder;
    audio::CircularBufferSinkType m_UserCircularBufferSink;
    audio::MemoryPoolType m_UserCircularBufferSinkMemoryPool;
    void* m_UserCircularBufferSinkBuffer;
    size_t m_UserCircularBufferSinkBufferSize;
    CircularBufferSinkState m_UserCircularBufferSinkState;
    bool m_IsCompatibleBusVolumeEnabled;
    bool m_IsUserThreadRenderingEnabled;
};
static_assert(sizeof(HardwareManager) == 0xa58);
} // namespace nn::atk::detail::driver