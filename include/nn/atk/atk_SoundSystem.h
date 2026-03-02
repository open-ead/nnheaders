#pragma once

#include <nn/time.h>
#include <nn/types.h>
#include <nn/audio/audio_MemoryPoolTypes.h>
#include <vapours/results/results_common.hpp>

#include <nn/atk/atk_Global.h>
#include <nn/atk/detail/dsp/atk_HardwareManager.h>
#include <nn/atk/detail/thread/atk_ThreadInfoReader.h>
#include <nn/atk/effect/atk_EffectAux.h>
#include <nn/atk/effect/atk_EffectBase.h>
#include <nn/atk/util/atk_AudioRendererPerformanceReader.h>
#include <nn/atk/util/atk_ProfileReader.h>

namespace nn::atk {
struct SoundSystem {
    static bool g_IsInitialized;
    static bool g_IsStreamLoadWait;
    static bool g_IsEnterSleep;
    static bool g_IsInitializedDriverCommandManager;
    
    static uintptr_t g_LoadThreadStackPtr;
    static size_t g_LoadThreadStackSize;

    static uintptr_t g_SoundThreadStackPtr;
    static size_t g_SoundThreadStackSize;

    static uintptr_t g_PerformanceFrameBuffer;
    static size_t g_PerformanceFrameBufferSize;

    static size_t g_SoundThreadCommandBufferSize;
    static size_t g_TaskThreadCommandBufferSize;
    static size_t g_VoiceCommandBufferSize;

    static uintptr_t g_MemoryPoolForSoundSystem;

    static s32 g_RendererSampleRate;
    static s32 g_CustomSubMixSubMixCount;
    static s32 g_CustomSubMixMixBufferCount;

    static bool g_IsProfilerEnabled;
    static bool g_IsDetailSoundThreadProfilerEnabled;
    static bool g_IsAdditionalEffectBusEnabled;
    static bool g_IsAdditionalSubMixEnabled;
    static bool g_IsEffectEnabled;
    static bool g_IsRecordingEnabled;
    static bool g_IsCircularBufferSinkEnabled;
    static bool g_IsCircularBufferSinkWarningDisplayed;
    static bool g_IsVoiceDropEnabled;
    static bool g_IsPreviousSdkVersionLowPassFilterCompatible;
    static bool g_IsUnusedEffectChannelMutingEnabled;
    static bool g_IsUserThreadRenderingEnabled;
    static bool g_IsCustomSubMixEnabled;
    static bool g_IsMemoryPoolAttachCheckEnabled;
    static bool g_IsBusMixVolumeEnabled;
    static bool g_IsVolumeThroughModeEnabled;

    constexpr static u32 VoiceCountMax = 96;
    constexpr static u32 WorkMemoryAlignSize = 4;
    constexpr static u32 VoiceCommandManagerCountMax = 2;
    constexpr static u32 SoundThreadIntervalUsec = 5000;

    constexpr static s32 g_TaskThreadFsPriority = 1;
    constexpr static bool g_IsStreamOpenFailureHalt = true;
    constexpr static bool g_IsTaskThreadEnabled = true;
    constexpr static bool g_IsManagingMemoryPool = true;
    constexpr static u32 g_UserEffectCount = 10;
    constexpr static bool g_IsSubMixEnabled = true;
    constexpr static bool g_IsPresetSubMixEnabled = true;
    constexpr static bool g_IsStereoModeEnabled = true;
    constexpr static bool g_IsSoundThreadEnabled = true;
    constexpr static s32 g_BusCountMax = 4;

    struct SoundSystemParam {
        constexpr static u32 DefaultSoundThreadPriority = 0;
        constexpr static u32 DefaultTaskThreadPriority = 0;
        constexpr static u32 DefaultSoundThreadStackSize = 0x4000;
        constexpr static u32 DefaultTaskThreadStackSize = 0x4000;
        constexpr static u32 DefaultSoundThreadCommandBufferSize = 0x20000;
        constexpr static u32 DefaultTaskThreadCommandBufferSize = 0x2000;
        constexpr static u32 DefaultUserEffectCount = 10;
        constexpr static u32 DefaultVoiceCountMax = 96;
        constexpr static u32 DefaultSoundThreadCoreNumber = 0;
        constexpr static u32 DefaultTaskThreadCoreNumber = 0;
        constexpr static u32 DefaultRecordingAudioFrameCount = 8;

        SoundSystemParam();

        s32 soundThreadPriority{4};
        size_t soundThreadStackSize{0x4000};
        size_t soundThreadCommandBufferSize{0x20000};
        size_t voiceCommandBufferSize;
        s32 taskThreadPriority{3};
        size_t taskThreadStackSize{0x4000};
        size_t taskThreadCommandBufferSize{0x2000};
        FsPriority taskThreadFsPriority{FsPriority_Normal};
        bool enableNwRenderer{};
        u32 nwVoiceSynthesizeBufferCount;
        s32 rendererSampleRate{48000};
        s32 effectCount{10};
        s32 voiceCountMax{96};
        s32 voiceCommandWaveBufferPacketCount{0x200};
        bool enableProfiler{false};
        bool enableDetailSoundThreadProfile{false};
        bool enableRecordingFinalOutputs{false};
        bool enableCircularBufferSink{false};
        s32 recordingAudioFrameCount{8};
        s32 soundThreadCoreNumber;
        s32 taskThreadCoreNumber;
        bool enableAdditionalEffectBus{false};
        bool enableAdditionalSubMix{false};
        bool enableTaskThread{true};
        bool enableSoundThread{true};
        bool enableMemoryPoolManagement{true};
        bool enableCircularBufferSinkBufferManagement{true};
        bool enableEffect{true};
        bool enableSubMix{true};
        bool enableStereoMode{false};
        bool enableVoiceDrop{true};
        bool enableCompatibleDownMixSetting{false};
        bool enableCompatibleLowPassFilter;
        bool enableUnusedEffectChannelMuting;
        bool enableCompatibleBusVolume;
        bool enableUserThreadRendering;
        bool enableCustomSubMix;
        s32 subMixCount{0};
        s32 subMixTotalChannelCount{0};
        s32 mixBufferCount{-1};
        s32 busCountMax{4};
        bool enableMemoryPoolAttachCheck{false};
        bool enableBusMixVolume{false};
        bool enableVolumeThroughMode{false};
    };
    static_assert(sizeof(SoundSystemParam) == 0x88);

    struct InitializeBufferSet {
        uintptr_t workMem;
        size_t workMemSize;
        uintptr_t memoryPoolMem;
        size_t memoryPoolMemSize;
        uintptr_t circularBufferSinkMem;
        size_t circularBufferSinkMemSize;
    };
    static_assert(sizeof(InitializeBufferSet) == 0x30);

    static size_t GetRequiredMemSize(const SoundSystemParam& param);
    static size_t GetRequiredMemSizeForCircularBufferSink(const SoundSystemParam& param);
    static size_t GetRequiredMemSizeForMemoryPool(const SoundSystemParam& param);
    
    static void SetupHardwareManagerParameter(detail::driver::HardwareManager::HardwareManagerParameter* pOutValue, 
                                              SoundSystemParam* parameter);

    static bool detail_InitializeSoundSystem(Result* pOutResult, const SoundSystemParam& param, const InitializeBufferSet& bufferSet);
    static void detail_InitializeDriverCommandManager(const SoundSystemParam& param, u64, u64, u64, u64);
    
    static bool Initialize(SoundSystemParam* param, uintptr_t workMem, size_t workMemSize);
    static bool Initialize(Result* pOutResult, const SoundSystemParam& param, 
                           uintptr_t workMem, size_t workMemSize);
        
    static void SetupInitializeBufferSet(InitializeBufferSet* pOutValue, 
                                         SoundSystemParam* param, InitializeBufferSet* bufferSet);

    static bool Initialize(SoundSystemParam* param, uintptr_t workMem, size_t workMemSize, 
                           uintptr_t memoryPoolMem, size_t memoryPoolMemSize);
    static bool Initialize(Result* pOutResult, const SoundSystemParam& param, uintptr_t workMem, 
                           size_t workMemSize, uintptr_t memoryPoolMem, size_t memoryPoolMemSize);

    static bool Initialize(const SoundSystemParam& param, const InitializeBufferSet& bufferSet);
    static bool Initialize(Result* pOutResult, const SoundSystemParam& param, const InitializeBufferSet& bufferSet);

    static void Finalize();

    static void SetSoundThreadBeginUserCallback(void(*threadBeginUserCallback)(u64), uintptr_t threadBeginUserCallbackArg);
    static void ClearSoundThreadBeginUserCallback();

    static void SetSoundThreadEndUserCallback(void(*threadEndUserCallback)(u64), uintptr_t threadEndUserCallbackArg);
    static void ClearSoundThreadEndUserCallback();

    static bool IsInitialized();

    static void SuspendAudioRenderer(TimeSpan timeSpan);
    static void ResumeAudioRenderer(TimeSpan timeSpan);

    static void ExecuteRendering();

    static void AttachMemoryPool(audio::MemoryPoolType* pMemoryPool, void* address, size_t size);
    static void DetachMemoryPool(audio::MemoryPoolType* pMemoryPool);

    static void DumpMemory();
    
    static size_t GetAudioRendererBufferSize();

    static void SetupHardwareManagerParameterFromCurrentSetting(detail::driver::HardwareManager::HardwareManagerParameter* pHardwareManagerParameter);

    static size_t GetRecorderBufferSize();
    static size_t GetUserCircularBufferSinkBufferSize();
    static size_t GetLowLevelVoiceAllocatorBufferSize();
    static size_t GetMultiVoiceManagerBufferSize();
    static size_t GetChannelManagerBufferSize();
    static size_t GetDriverCommandBufferSize();
    static size_t GetAllocatableDriverCommandSize();
    static size_t GetAllocatedDriverCommandBufferSize();
    static size_t GetAllocatedDriverCommandCount();

    static void RegisterAudioRendererPerformanceReader(AudioRendererPerformanceReader& audioRendererPerformanceReader);

    static bool AppendEffect(AuxBus auxBus, EffectBase* pEffectBase, 
                             void* buffer, size_t bufferSize);
    static bool AppendEffect(AuxBus auxBus, EffectBase* pEffectBase, 
                             void* buffer, size_t bufferSize, OutputDevice device);
    static bool AppendEffect(AuxBus auxBus, EffectBase* pEffectBase, 
                             void* buffer, size_t bufferSize, OutputDevice device, s32 subMixNumber);

    static bool AppendEffect(AuxBus auxBus, EffectAux* pEffectAux, 
                             void* buffer, size_t bufferSize);
    static bool AppendEffect(AuxBus auxBus, EffectAux* pEffectAux, 
                             void* buffer, size_t bufferSize, OutputDevice device);
    static bool AppendEffect(AuxBus auxBus, EffectAux* pEffectAux, 
                             void* buffer, size_t bufferSize, OutputDevice device, s32 subMixNumber);

    static bool AppendEffectToFinalMix(EffectAux* pEffectAux, void* buffer, size_t bufferSize);
    static bool AppendEffectToAdditionalSubMix(EffectAux* pEffectAux, void* buffer, size_t bufferSize);

    static size_t GetRequiredEffectAuxBufferSize(const EffectAux* pEffectAux);

    static void RemoveEffect(AuxBus auxBus, EffectBase* pEffectBase);
    static void RemoveEffect(AuxBus auxBus, EffectBase* pEffectBase, OutputDevice outputDevice);
    static void RemoveEffect(AuxBus auxBus, EffectBase* pEffectBase, OutputDevice outputDevice, 
                             s32 subMixNumber);

    static void RemoveEffect(AuxBus auxBus, EffectAux* pEffectAux);
    static void RemoveEffect(AuxBus auxBus, EffectAux* pEffectAux, OutputDevice outputDevice);
    static void RemoveEffect(AuxBus auxBus, EffectAux* pEffectAux, OutputDevice outputDevice, 
                             s32 subMixNumber);

    static void RemoveEffectFromFinalMix(EffectAux* pEffectAux);
    static void RemoveEffectFromAdditionalSubMix(EffectAux* pEffectAux);

    static void ClearEffect(AuxBus auxBus);
    static void ClearEffect(AuxBus auxBus, OutputDevice outputDevice);
    static void ClearEffect(AuxBus auxBus, OutputDevice outputDevice, s32 subMixNumber);
    
    static void ClearEffectFromFinalMix();
    static void ClearEffectFromAdditionalSubMix();

    static bool IsClearEffectFinished(AuxBus auxBus);
    static bool IsClearEffectFinished(AuxBus auxBus, OutputDevice outputDevice);
    static bool IsClearEffectFinished(AuxBus auxBus, OutputDevice outputDevice, s32 subMixNumber);

    static bool IsClearEffectFromFinalMixFinished();
    static bool IsClearEffectFromAdditionalSubMixFinished();

    static void SetAuxBusVolume(AuxBus auxBus, f32 volume, TimeSpan timeSpan);
    static void SetAuxBusVolume(AuxBus auxBus, f32 volume, TimeSpan timeSpan, s32);

    static f32 GetAuxBusVolume(AuxBus auxBus);
    static f32 GetAuxBusVolume(AuxBus auxBus, s32 subMixIndex);

    static void SetMainBusChannelVolumeForAdditionalEffect(f32 volume, s32 srcChannel, s32 dstChannel);
    static f32 GetMainBusChannelVolumeForAdditionalEffect(s32 srcChannel, s32 dstChannel);

    static void SetAuxBusChannelVolumeForAdditionalEffect(AuxBus auxBus, f32 volume, s32 srcChannel, s32 dstChannel);
    static f32 GetAuxBusChannelVolumeForAdditionalEffect(AuxBus auxBus, s32 srcChannel, s32 dstChannel);

    static void SetAllAuxBusChannelVolumeForAdditionalEffect(f32 volume, s32 srcChannel, s32 dstChannel);
    static void SetAllBusChannelVolumeForAdditionalEffect(f32 volume, s32 srcChannel, s32 dstChannel);

    static void VoiceCommandProcess(UpdateType updateType, u32);
    static void VoiceCommandProcess(u32);
    
    static void VoiceCommandUpdate();

    static size_t GetPerformanceFrameBufferSize();

    static s32 GetDroppedLowLevelVoiceCount();
    
    static void RegisterSoundThreadUpdateProfileReader(AtkProfileReader<SoundThreadUpdateProfile>&);
    static void UnregisterSoundThreadUpdateProfileReader(AtkProfileReader<SoundThreadUpdateProfile>&);

    static void RegisterSoundThreadInfoRecorder(detail::ThreadInfoRecorder&);
    static void UnregisterSoundThreadInfoRecorder(detail::ThreadInfoRecorder&);

    static bool ReadCircularBufferSink(void* buffer, size_t bufferSize);

    static size_t GetCircularBufferSinkBufferSize();

    static u32 GetRendererSampleCount();
    static u32 GetRendererChannelCountMax();

    static void StopCircularBufferSink();
    static void StartCircularBufferSink();

    static CircularBufferSinkState GetCircularBufferSinkState();
    static detail::SoundInstanceConfig GetSoundInstanceConfig();
};
} // namespace nn::atk