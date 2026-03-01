#pragma once

#include <nn/audio/audio_SubMixTypes.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/detail/atk_MoveValue.h>
#include <nn/atk/submix/atk_OutputMixer.h>

namespace nn::atk {
namespace detail::driver {
class HardwareManager;
};

class SubMix : OutputMixer {
public:
    class VolumeData {
    public:
        VolumeData();

        bool Update();

    private:
        detail::MoveValue<f32, s32> m_Volume;
        bool m_IsMute;
        bool m_IsPrevMute;
        bool m_IsDirtyFlag;
    };
    static_assert(sizeof(VolumeData) == 0x14);

    class SubMixParam {
    public:
        SubMixParam();

        void SetSrcBusCount(s32 srcBusCount);
        s32 GetSrcBusCount() const;

        void SetSrcChannelCount(s32 srcChannelCount);
        s32 GetSrcChannelCount() const;

        void SetDstBusCount(s32 dstBusCount);
        s32 GetDstBusCount() const;

        void SetDstChannelCount(s32 dstChannelCount);
        s32 GetDstChannelCount() const;

        void SetOutputReceiver(const OutputReceiver* pOutputReceiver);
        OutputReceiver* GetOutputReceiver() const;

        void SetEffectEnabled(bool isEffectEnabled);
        bool IsEffectEnabled() const;

        void SetSoundSendClampEnabled(s32 index, bool IsSoundSendClampEnabled);
        bool GetSoundSendClampEnabled(s32 index) const;

    private:
        s32 m_SrcBusCount;
        s32 m_SrcChannelCount;
        s32 m_DstBusCount;
        s32 m_DstChannelCount;
        OutputReceiver* m_pOutputReceiver;
        bool m_IsEffectEnabled;
        bool m_IsSoundSendClampEnabledArray[24];
    };
    static_assert(sizeof(SubMixParam) == 0x38);

    SubMix();
    static size_t GetRequiredMemorySize(s32 srcBusCount, s32 srcChannelCount, 
                                        s32 dstBusCount, s32 dstChannelCount);
    static size_t GetRequiredMemorySize(s32 srcBusCount, s32 srcChannelCount, 
                                        s32 dstBusCount, s32 dstChannelCount, 
                                        bool isEffectEnabled);
    static size_t GetRequiredMemorySize(s32 srcBusCount, s32 srcChannelCount, 
                                        s32 dstBusCount, s32 dstChannelCount, 
                                        bool isEffectEnabled, bool isInternalCall);
    
    static size_t GetRequiredMemorySizeImpl(const SubMixParam& param);

    static size_t GetRequiredMemorySize(s32 srcBusCount, s32 dstBusCount, 
                                        const OutputReceiver* pReceiver, 
                                        bool isEffectEnabled);

    static size_t GetRequiredMemorySize(const SubMixParam& param);

    bool Initialize(s32 srcBusCount, s32 srcChannelCount, s32 dstBusCount, 
                    s32 dstChannelCount, void* buffer, size_t bufferSize);
    bool Initialize(s32 srcBusCount, s32 srcChannelCount, s32 dstBusCount, 
                    s32 dstChannelCount, bool isEffectEnabled, void* buffer, 
                    size_t bufferSize);
    bool Initialize(s32 srcBusCount, s32 srcChannelCount, s32 dstBusCount, 
                    s32 dstChannelCount, bool isEffectEnabled, bool isInternalCall,
                    void* buffer, size_t bufferSize);
    
    bool InitializeImpl(const SubMixParam& param, void* buffer, size_t bufferSize);
    bool InitializeImpl(const SubMixParam& param, void* buffer, size_t bufferSize, bool isInternalCall);

    bool Initialize(s32 srcBusCount, s32 dstBusCount, const OutputReceiver* pReceiver, 
                    void* buffer, size_t bufferSize);
    bool Initialize(s32 srcBusCount, s32 dstBusCount, const OutputReceiver* pReceiver, 
                    bool isEffectEnabled, void* buffer, size_t bufferSize);
    bool Initialize(const SubMixParam& param, void* buffer, size_t bufferSize);

    void Finalize();

    bool IsRemovable() const;

    void Update();

    void UpdateBusMixVolume(s32 bus);
    void UpdateChannelMixVolume(s32 bus);
    void UpdateMixVolume(s32 srcBus, s32 srcChannel,s32 dstBus,s32 dstChannel);

    void SetDestination(OutputReceiver* pReceiver);
    void ApplyDestination();

    f32 GetSend(s32 srcBus, s32 dstBus) const;
    f32 GetSendImpl(s32 srcBus, s32 srcChannel,s32 dstBus,s32 dstChannel) const;

    void SetSend(s32 srcBus, s32 dstBus, f32 send);
    void SetSendImpl(s32 srcBus, s32 srcChannel,s32 dstBus,s32 dstChannel, f32 send);

    void SetBusVolume(s32 bus, f32 volume, s32 fadeFrame);
    f32 GetBusVolume(s32 bus) const;

    void SetBusMute(s32 bus, bool isMute);
    bool IsBusMuted(s32 bus) const;

    bool IsSoundSendClampEnabled(s32 bus) const override;

    void SetChannelVolume(s32 channel, f32 volume, s32 fadeFrame);
    f32 GetChannelVolume(s32 channel) const;

    void SetChannelMute(s32 channel, bool isMute);
    bool IsChannelMuted(s32 channel) const;

    void SetSubMixVolume(f32 volume, s32 fadeFrame);
    f32 GetSubMixVolume() const;

    void SetSubMixMute(bool isMute);
    bool IsSubMixMuted() const;

    void SetMuteUnusedEffectChannel(bool isUnusedEffectChannelMuted);
    void IsUnusedEffectChannelMuted() const;

    void MuteUnusedEffectChannel(ChannelIndex* effectChannelIndex, s32 effectChannelCount, s32 bus);

    bool AppendEffect(EffectBase* pEffect, s32 bus, void* buffer, size_t bufferSize);
    bool AppendEffect(EffectAux* pEffect, s32 bus, void* buffer, size_t bufferSize);

    bool RemoveEffect(EffectBase* pEffect, s32 bus);
    bool RemoveEffect(EffectAux* pEffect, s32 bus);

    bool ClearEffect(s32 bus);

    bool IsEffectEnabled() const;

    void AppendEffectImpl(EffectBase* pEffect, s32 bus, void* buffer, size_t bufferSize) override;
    void AppendEffectImpl(EffectAux* pEffect, s32 bus, void* buffer, size_t bufferSize) override;

    ReceiverType GetReceiverType() const override;
    s32 GetChannelCount() const override;
    s32 GetBusCount() const override;

    void AddReferenceCount(s32 value) override;

private:
    friend detail::driver::HardwareManager;

    util::IntrusiveListNode m_Link;
    audio::SubMixType m_SubMix;
    std::atomic_uint m_ReferenceCount;
    OutputReceiver* m_pReceiver;
    VolumeData* m_pBusVolume;
    VolumeData* m_pChannelVolume;
    VolumeData m_SubMixVolume;
    bool* m_pIsSoundSendClampEnabledArray;
    f32** m_ppSendVolume;
    s32 m_ChannelCount;
    s32 m_BusCount;
    s32 m_ReceiverChannelCountMax;
    detail::fnd::CriticalSection m_VolumeLock;
    detail::fnd::CriticalSection m_DestinationLock;
    bool m_IsInitialized;
    bool m_IsUnusedEffectChannelMuted;
    bool m_IsAppliedOutputReceiver;
};
static_assert(sizeof(SubMix) == 0xf8);
} // namespace nn::atk