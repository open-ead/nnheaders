#pragma once

#include <nn/time.h>
#include <nn/types.h>
#include <vapours/results/results_common.hpp>

#include <nn/atk/atk_Global.h>
#include <nn/atk/util/atk_AudioRendererPerformanceReader.h>

namespace nn::audio {
struct MemoryPoolType;
} // namespace nn::audio

namespace nn::atk {
struct SoundSystem {
    static bool g_IsInitialized;
    static bool g_IsStreamLoadWait;
    static bool g_IsInitializedDriverCommandManager;
    
    static std::uintptr_t g_LoadThreadStackPtr;
    static std::size_t g_LoadThreadStackSize;

    static std::uintptr_t g_SoundThreadStackPtr;
    static std::size_t g_SoundThreadStackSize;

    static std::uintptr_t g_PerformanceFrameBuffer;
    static std::size_t g_PerformanceFrameBufferSize;

    static std::size_t g_SoundThreadCommandBufferSize;
    static std::size_t g_TaskThreadCommandBufferSize;

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

    const static s32 g_TaskThreadFsPriority; // = 1;
    const static bool g_IsStreamOpenFailureHalt; // = true;
    const static bool g_IsTaskThreadEnabled; // = true;
    const static bool g_IsManagingMemoryPool; // = true;
    const static u32 g_VoiceCountMax; // = 96;
    const static u32 g_UserEffectCount; // = 10;
    const static bool g_IsSubMixEnabled; // = true;
    const static bool g_IsPresetSubMixEnabled; // = true;
    const static bool g_IsStereoModeEnabled; // = true;
    const static bool g_IsSoundThreadEnabled; // = true;
    const static s32 g_BusCountMax; // = 4;

    struct SoundSystemParam {
        s32 soundThreadPriority{4};
        std::size_t soundThreadStackSize{0x4000};
        std::size_t soundThreadCommandBufferSize{0x20000};
        std::size_t voiceCommandBufferSize;
        s32 taskThreadPriority{3};
        std::size_t taskThreadStackSize{0x4000};
        std::size_t taskThreadCommandBufferSize{0x2000};
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

        SoundSystemParam();
    };
    static_assert(sizeof(SoundSystemParam) == 0x88);

    struct InitializeBufferSet {
        std::uintptr_t workMem;
        std::size_t workMemSize;
        std::uintptr_t memoryPoolMem;
        std::size_t memoryPoolMemSize;
        std::uintptr_t circularBufferSinkMem;
        std::size_t circularBufferSinkMemSize;
    };
    static_assert(sizeof(InitializeBufferSet) == 0x30);

    static std::size_t GetRequiredMemSize(const SoundSystemParam& param);
    static std::size_t GetRequiredMemSizeForCircularBufferSink(const SoundSystemParam& param);
    static std::size_t GetRequiredMemSizeForMemoryPool(const SoundSystemParam& param);
    
    static void SetupHardwareManagerParameter(class HardwareManagerParameter* pOutValue, SoundSystemParam* parameter);

    static bool detail_InitializeSoundSystem(Result* pOutResult, const SoundSystemParam& param, const InitializeBufferSet& bufferSet);
    static void detail_InitializeDriverCommandManager(const SoundSystemParam& param, u64, u64, u64, u64);
    
    static bool Initialize(SoundSystemParam* param, std::uintptr_t workMem, std::size_t workMemSize);
    static bool Initialize(Result* pOutResult, const SoundSystemParam& param, std::uintptr_t workMem, std::size_t workMemSize);
        
    static void SetupInitializeBufferSet(InitializeBufferSet* pOutValue, SoundSystemParam* param, InitializeBufferSet* bufferSet);

    static bool Initialize(SoundSystemParam* param, u64, u64, u64, u64);
    static bool Initialize(Result* pOutResult, const SoundSystemParam& param, u64, u64, u64, u64);

    static bool Initialize(const SoundSystemParam& param, const InitializeBufferSet& bufferSet);
    static bool Initialize(Result* pOutResult, const SoundSystemParam& param, const InitializeBufferSet& bufferSet);

    static void Finalize();

    static void SetSoundThreadBeginUserCallback(void(*threadBeginUserCallback)(u64), std::uintptr_t threadBeginUserCallbackArg);
    static void ClearSoundThreadBeginUserCallback();

    static void SetSoundThreadEndUserCallback(void(*threadEndUserCallback)(u64), std::uintptr_t threadEndUserCallbackArg);
    static void ClearSoundThreadEndUserCallback();

    static bool IsInitialized();

    static void SuspendAudioRenderer(TimeSpan timeSpan);
    static void ResumeAudioRenderer(TimeSpan timeSpan);

    static void AttachMemoryPool(audio::MemoryPoolType* pMemoryPool, void* address, std::size_t size);
    static void DetachMemoryPool(audio::MemoryPoolType* pMemoryPool);

    static void DumpMemory();
    
    static std::size_t GetAudioRendererBufferSize();

    // void SetupHardwareManagerParameterFromCurrentSetting(detail::driver::HardwareManager::HardwareManagerParameter* pHardwareManagerParameter);

    static std::size_t GetRecorderBufferSize();
    static std::size_t GetUserCircularBufferSinkBufferSize();
    static std::size_t GetLowLevelVoiceAllocatorBufferSize();
    static std::size_t GetMultiVoiceManagerBufferSize();
    static std::size_t GetChannelManagerBufferSize();
    static std::size_t GetDriverCommandBufferSize();
    static std::size_t GetAllocatableDriverCommandSize();
    static std::size_t GetAllocatedDriverCommandBufferSize();
    static std::size_t GetAllocatedDriverCommandCount();

    static void RegisterAudioRendererPerformanceReader(AudioRendererPerformanceReader& audioRendererPerformanceReader);

    // static bool AppendEffect(AuxBus auxBus, EffectBase* pEffectBase, void*, u64);
    // static bool AppendEffect(AuxBus auxBus, EffectBase* pEffectBase, void*, u64, OutputDevice);
    // static bool AppendEffect(AuxBus auxBus, EffectBase* pEffectBase, void*, u64, OutputDevice, s32);

    // static bool AppendEffect(AuxBus auxBus, EffectAux* pEffectAux, void*, u64);
    // static bool AppendEffect(AuxBus auxBus, EffectAux* pEffectAux, void*, u64, OutputDevice);
    // static bool AppendEffect(AuxBus auxBus, EffectAux* pEffectAux, void*, u64, OutputDevice, s32);

    // static bool AppendEffectToFinalMix(EffectAux* pEffectAux, void*, u64);
    // static bool AppendEffectToAdditionalSubMix(EffectAux* pEffectAux, void*, u64);

    // static std::size_t GetRequiredEffectAuxBufferSize(EffectAux* pEffectAux);

    // static void RemoveEffect(AuxBus auxBus, EffectBase* pEffectBase);
    // static void RemoveEffect(AuxBus auxBus, EffectBase* pEffectBase, OutputDevice outputDevice);
    // static void RemoveEffect(AuxBus auxBus, EffectBase* pEffectBase, OutputDevice outputDevice, s32);

    // static void RemoveEffect(AuxBus auxBus, EffectAux* pEffectAux);
    // static void RemoveEffect(AuxBus auxBus, EffectAux* pEffectAux, OutputDevice outputDevice);
    // static void RemoveEffect(AuxBus auxBus, EffectAux* pEffectAux, OutputDevice outputDevice, s32);

    // static void RemoveEffectFromFinalMix(EffectAux* pEffectAux);
    // static void RemoveEffectFromAdditionalSubMix(EffectAux* pEffectAux);

    static void ClearEffect(AuxBus auxBus);
    static void ClearEffect(AuxBus auxBus, OutputDevice outputDevice);
    static void ClearEffect(AuxBus auxBus, OutputDevice outputDevice, s32);
    
    static void ClearEffectFromFinalMix();
    static void ClearEffectFromAdditionalSubMix();

    static bool IsClearEffectFinished(AuxBus auxBus);
    static bool IsClearEffectFinished(AuxBus auxBus, OutputDevice outputDevice);
    static bool IsClearEffectFinished(AuxBus auxBus, OutputDevice outputDevice, s32);

    static bool IsClearEffectFromFinalMixFinished();
    static bool IsClearEffectFromAdditionalSubMixFinished();

    static void SetAuxBusVolume(AuxBus auxBus, f32 volume, TimeSpan timeSpan);
    static void SetAuxBusVolume(AuxBus auxBus, f32 volume, TimeSpan timeSpan, s32);

    static f32 GetAuxBusVolume(AuxBus auxBus);
    static f32 GetAuxBusVolume(AuxBus auxBus, s32);

    static void VoiceCommandProcess(UpdateType updateType, u32);
    static void VoiceCommandProcess(u32);
    
    static void VoiceCommandUpdate();

    static std::size_t GetPerformanceFrameBufferSize();

    static s32 GetDroppedLowLevelVoiceCount();
    
    // static void RegisterSoundThreadUpdateProfileReader(AtkProfileReader<nn::atk::SoundThreadUpdateProfile>&);
    // static void UnregisterSoundThreadUpdateProfileReader(AtkProfileReader<nn::atk::SoundThreadUpdateProfile>&);

    static bool ReadCircularBufferSink(void*, u64);

    static std::size_t GetCircularBufferSinkBufferSize();

    static u32 GetRendererSampleCount();
    static u32 GetRendererChannelCountMax();

    static detail::SoundInstanceConfig GetSoundInstanceConfig();
};
} // namespace nn::atk