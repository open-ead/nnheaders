#pragma once

#include <nn/atk/detail/util/atk_InstancePool.h>
#include <nn/atk/detail/voice/atk_Channel.h>

namespace nn::atk::detail::driver {
class ChannelManager {
public:
    using ChannelPool = InstancePool<Channel>;
    using ChannelList = util::IntrusiveList<Channel, 
                            util::IntrusiveListMemberNodeTraits<Channel, &Channel::m_Link>>;

    ChannelManager();

    size_t GetObjectSize(const SoundInstanceConfig& config);
    size_t GetRequiredMemSize(s32 channelCount, const SoundInstanceConfig& config);

    void Initialize(void* mem, size_t memSize, s32 channelCount, const SoundInstanceConfig* config);
    void Finalize();

    void Free(Channel* channel);

    Channel* Alloc();

    void UpdateAllChannel();
    void UpdateAudioFrameChannel();

private:
    ChannelPool m_Pool;
    ChannelList m_ChannelList;
    bool m_IsInitialized;
    s32 m_ChannelCount;
    InstancePool<OutputAdditionalParam>* m_pAdditionalParamPool;
    BufferPool* m_pAdditionalParamBufferPool;
    std::size_t m_AdditionalParamBufferSizePerChannel;
    SoundInstanceConfig m_SoundInstanceConfig;
};
static_assert(sizeof(ChannelManager) == 0x50);
} // namespace nn::atk::detail::driver