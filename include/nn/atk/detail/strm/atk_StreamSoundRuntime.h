#pragma once

#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/detail/atk_StartInfoReader.h>
#include <nn/atk/detail/strm/atk_StreamSound.h>

namespace nn::atk::detail {
class StreamSoundRuntime {
public:
    constexpr static u32 DefaultStreamBlockCount = 5;

    StreamSoundRuntime();
    ~StreamSoundRuntime();

    bool Initialize(s32 soundCount, void** pOutAllocatedAddr, const void* endAddr, 
                    void* streamInstanceBuffer, std::size_t streamInstanceBufferSize);
    
    static std::size_t GetRequiredStreamInstanceSize(s32 soundCount);

    bool SetupStreamBuffer(const SoundArchive* pSoundArchive, void* strmBuffer, 
                           std::size_t strmBufferSize);
    bool SetupStreamBuffer(const SoundArchive* pSoundArchive, void* strmBuffer, 
                           std::size_t strmBufferSize, driver::StreamBufferPool* strmBufferPool);

    std::size_t GetRequiredStreamBufferSize(const SoundArchive* soundArchive) const;
    static u32 GetRequiredStreamBufferTimes(const SoundArchive* soundArchive);

    bool SetupStreamCacheBuffer(const SoundArchive* pSoundArchive, void* streamCacheBuffer, 
                                std::size_t streamCacheSize);

    void Finalize();

    static std::size_t GetRequiredMemorySize(const SoundArchive::SoundArchivePlayerInfo& soundArchivePlayerInfo,
                                             s32 alignment);

    static std::size_t GetRequiredStreamCacheSize(const SoundArchive* pSoundArchive, 
                                                  std::size_t);

    s32 GetActiveCount() const;
    s32 GetActiveChannelCount() const;
    s32 GetActiveTrackCount() const;
    s32 GetFreeCount() const;

    void SetupUserParam(void** pOutAllocatedAddr, std::size_t adjustSize);

    void Update();
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    StreamSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, s32 ambientPriority, 
                            BasicSound::AmbientInfo* ambientArgInfo);
#else
    StreamSound* AllocSound(SoundArchive::ItemId soundId, s32 priority, s32 ambientPriority, 
                            BasicSound::AmbientInfo* ambientArgInfo, OutputReceiver* pOutputReceiver);                       
#endif
    SoundStartable::StartResult PrepareImpl(const SoundArchive* pSoundArchive, 
                                            const SoundDataManager* pSoundDataManager, 
                                            SoundArchive::ItemId soundId, 
                                            StreamSound* sound, 
                                            const SoundArchive::SoundInfo* commonInfo, 
                                            const StartInfoReader& startInfoReader);

    void DumpMemory(const SoundArchive* pSoundArchive) const;

private:
    StreamSoundInstanceManager m_StreamSoundInstanceManager;
    driver::StreamSoundLoaderManager m_StreamSoundLoaderManager;
    driver::StreamBufferPool m_StreamBufferPool;
    SoundArchiveFilesHook* m_pSoundArchiveFilesHook;
    s32 m_StreamBlockCount;
};
static_assert(sizeof(StreamSoundRuntime) == 0xb8);
}  // namespace nn::atk::detail
