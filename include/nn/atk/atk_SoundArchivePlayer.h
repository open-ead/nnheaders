#pragma once

#include <nn/audio/audio_MemoryPoolTypes.h>

#include <nn/atk/detail/atk_AdvancedWaveSoundRuntime.h>
#include <nn/atk/detail/seq/atk_SequenceSoundRuntime.h>
#include <nn/atk/detail/strm/atk_StreamSoundRuntime.h>
#include <nn/atk/detail/wsd/atk_WaveSoundRuntime.h>

namespace nn::atk {
class SoundArchivePlayer : SoundStartable {
public:
    constexpr static u32 BufferAlignSize = 4096;
    constexpr static u32 StreamBufferTimesMax = 4;
    constexpr static u8 UserParamBoundary = 4;

    struct InitializeParam {
        SoundArchive* pSoundArchive;
        SoundDataManager* pSoundDataManager;
        void* pSetupBuffer;
        std::size_t setupBufferSize;
        void* pStreamBuffer;
        std::size_t streamBufferSize;
        void* pStreamCacheBuffer;
        std::size_t streamCacheSize;
        bool enablePreparingStreamInstanceBufferFromSetupBuffer;
        void* pStreamInstanceBuffer;
        std::size_t streamInstanceBufferSize;
        std::size_t userParamSizePerSound;
        s32 addonSoundArchiveCount;
    };
    static_assert(sizeof(InitializeParam) == 0x68);

    struct StreamSoundInstanceState {
        s32 activeStreamSoundInstanceCount;
        s32 activeStreamChannelCount;
        s32 activeStreamTrackCount;
    };
    static_assert(sizeof(StreamSoundInstanceState) == 0xc);

    SoundArchivePlayer();
    ~SoundArchivePlayer() override;

    bool IsAvailable() const;

    bool Initialize(const SoundArchive* arc, const SoundDataManager* manager, void* buffer, 
                    size_t size, void* strmBuffer, size_t strmBufferSize, 
                    size_t userParamSizePerSound);
    bool Initialize(const InitializeParam& param);

    void Finalize();

    void StopAllSound(s32, bool);

    void DisposeInstances();

    static size_t GetRequiredMemSize(const SoundArchive* arc);
    static size_t GetRequiredMemSize(const SoundArchive* arc, size_t userParamSizePerSound, 
                                          s32 addonSoundArchiveCount);
    static size_t GetRequiredMemSize(const SoundArchive* arc, size_t userParamSizePerSound);
    static size_t GetRequiredMemSize(const InitializeParam& param);

    static size_t GetRequiredStreamInstanceSize(const SoundArchive* arc);

    size_t GetRequiredStreamBufferSize(const SoundArchive* arc) const;
    size_t GetRequiredStreamBufferTimes(const SoundArchive* arc) const;
    
    static size_t GetRequiredStreamCacheSize(const SoundArchive* arc, size_t);


    bool SetupMram(const SoundArchive* pArc, void* buffer, size_t size, 
                   size_t userParamSizePerSound, s32 addonSoundArchiveCount, 
                   void* streamSoundInstanceBuffer, size_t streamSoundInstanceBufferSize);

    bool SetupSoundPlayer(const SoundArchive* pArc, void** pOutAllocatedAddr, const void* endAddr);
    bool SetupAddonSoundArchiveContainer(s32 containerCount, void** pOutAllocatedAddr, const void* endAddr);
    bool SetupUserParamForBasicSound(const SoundArchive::SoundArchivePlayerInfo& playerInfo, 
                                     void** pOutAllocatedAddr, const void* endAddr, size_t);

    detail::PlayerHeap* CreatePlayerHeap(void** pOutAllocatedAddr, const void* endAddr, size_t);

    void Update();
                       
    SoundPlayer* GetSoundPlayer(u32);
    
    SoundArchive* GetSoundArchive() const;
    AddonSoundArchive* GetAddonSoundArchive(const char*) const;
    AddonSoundArchive* GetAddonSoundArchive(s32) const;
    char* GetAddonSoundArchiveName(s32) const;
    os::Tick* GetAddonSoundArchiveAddTick(s32) const;
    SoundDataManager* GetAddonSoundDataManager(const char*) const;

    SoundPlayer* GetSoundPlayer(u32) const;
    SoundPlayer* GetSoundPlayer(const char*);
    SoundPlayer* GetSoundPlayer(const char*) const;

    void* detail_GetFileAddress(SoundArchive::FileId fileId) const;

    void AddAddonSoundArchive(const char*, const AddonSoundArchive*, const SoundDataManager*);
    void RemoveAddonSoundArchive(const AddonSoundArchive*);

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    void SetDefaultOutputReceiver(OutputReceiver* pOutputReceiver);
#endif

    StartResult detail_SetupSound(SoundHandle* handle, u32 soundId, bool holdFlag,
                                  const char* soundArchiveName, const StartInfo* startInfo) override;
    StartResult detail_SetupSoundImpl(SoundHandle* handle, u32 soundId, 
                                      detail::BasicSound::AmbientInfo* ambientArgInfo,
                                      SoundActor* actor, bool holdFlag, const char* soundArchiveName,
                                      const StartInfo* startInfo);

    bool IsSoundArchiveFileHooksEnabled() const;

    void LockSoundArchiveFileHooks();

    bool IsSequenceSoundEdited(const char*) const;
    bool IsStreamSoundEdited(const char*) const;
    bool IsWaveSoundEdited(const char*) const;

    void EnableHook(const SoundArchive*, bool);

    StartResult PreprocessSinglePlay(const SoundArchive::SoundInfo& info, u32 soundId, 
                                     SoundPlayer& player);

    void SetCommonSoundParam(detail::BasicSound* pSound, const SoundArchive::SoundInfo* info);

    void UnlockSoundArchiveFileHooks();

    void SetSequenceUserProcCallback(SequenceUserProcCallback callback, void* arg);

    static void SetSequenceSkipIntervalTick(s32 tick);
    static s32 GetSequenceSkipIntervalTick();

    Result ReadWaveSoundDataInfo(detail::WaveSoundDataInfo*, u32, const SoundArchive*,
                              const SoundDataManager*) const;
    Result ReadWaveSoundDataInfo(detail::WaveSoundDataInfo*, u32, const char*) const;
    Result ReadWaveSoundDataInfo(detail::WaveSoundDataInfo*, const char*, const char*) const;
    Result ReadWaveSoundDataInfo(detail::WaveSoundDataInfo*, u32) const;
    Result ReadWaveSoundDataInfo(detail::WaveSoundDataInfo*, const char*) const;

    Result ReadStreamSoundDataInfo(detail::StreamSoundDataInfo*, const SoundArchive*, u32) const;
    Result ReadStreamSoundDataInfo(detail::StreamSoundDataInfo*, u32, const char*) const;
    Result ReadStreamSoundDataInfo(detail::StreamSoundDataInfo*, const char*, const char*) const;
    Result ReadStreamSoundDataInfo(detail::StreamSoundDataInfo*, u32) const;
    Result ReadStreamSoundDataInfo(detail::StreamSoundDataInfo*, const char*) const;

    static size_t GetRequiredWorkBufferSizeToReadStreamSoundHeader();

    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, u32, 
                                         const char* const*, s32, const SoundArchive*, 
                                         void*, size_t) const;
    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, u32, const char*,
                                         void*, size_t) const;
    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, u32, 
                                         const char* const*, s32, void*, size_t, 
                                         const char*) const;
    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, const char*, 
                                         const char*, void*, size_t) const;
    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, const char*, 
                                         const char* const*, s32, void*, size_t,
                                         const char*) const;
    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, u32, 
                                         const char* const*, s32, void*, size_t) const;
    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, const char*, 
                                         const char* const*, s32, void*, size_t) const;
    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, const char*, 
                                         const char*, void*, size_t, 
                                         const char* soundArchiveName) const;
    Result ReadStreamSoundRegionDataInfo(detail::StreamSoundRegionDataInfo*, u32, const char*, 
                                         void*, size_t, const char* soundArchiveName) const;
                                      
    void DumpMemory() const;

    bool ReadStreamSoundInstanceState(StreamSoundInstanceState*) const;

    Result CheckStreamSoundFileExisting(u32) const;
    Result CheckStreamSoundFileExisting(u32, const char* soundArchiveName) const;
    Result CheckStreamSoundFileExisting(char* streamSoundName) const;
    Result CheckStreamSoundFileExisting(const char*, const char*) const;
    Result CheckStreamSoundFileExisting(const SoundArchive*, u32) const;
    
    SoundArchive::ItemId detail_GetItemId(char* pString) override;
    SoundArchive::ItemId detail_GetItemId(char* pString, const char* soundArchiveName) override;

private:
    detail::SoundArchiveManager m_SoundArchiveManager;
    u32 m_SoundPlayerCount;
    SoundPlayer* m_pSoundPlayers;
    detail::SequenceSoundRuntime m_SequenceSoundRuntime;
    detail::WaveSoundRuntime m_WaveSoundRuntime;
    detail::AdvancedWaveSoundRuntime m_AdvancedWaveSoundRuntime;
    detail::StreamSoundRuntime m_StreamSoundRuntime;
    size_t m_SoundUserParamSize;
    s32 m_ArchiveContainerCount;
    detail::AddonSoundArchiveContainer* m_pArchiveContainers;
    os::Tick m_AddonSoundArchiveLastAddTick;
    audio::MemoryPoolType m_MemoryPoolForStreamInstance;
    bool m_IsMemoryPoolForStreamInstanceAttached;
    audio::MemoryPoolType m_MemoryPoolForPlayerHeap;
    bool m_IsMemoryPoolForPlayerHeapAttached;
    detail::SoundArchiveFilesHook* m_pSoundArchiveFilesHook;
    bool m_IsEnableWarningPrint;
    bool m_IsInitialized;
    bool m_IsAdvancedWaveSoundEnabled;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    OutputReceiver* m_pDefaultOutputReceiver;
    u8 m_Padding[1];
#endif
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(SoundArchivePlayer) == 0x2e0);
#else
static_assert(sizeof(SoundArchivePlayer) == 0x310);
#endif
}  // namespace nn::atk
