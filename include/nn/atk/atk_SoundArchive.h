#pragma once

#include <nn/atk/atk_Config.h>
#include <nn/atk/atk_Util.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk {
namespace detail{
class SoundArchiveFileReader;
class SoundArchiveParametersHook;
struct SoundArchiveFilesHook;

namespace driver {
class StreamSoundLoader;
}

} // namespace nn::atk::detail

class SoundArchive {
public:
    using ItemId = u32;
    using FileId = ItemId;
    using StringId = ItemId;

    static const ItemId InvalidId = -1;

    static const int UserParamIndexMax = 3;
    static const u32 ResultInvalidSoundId = 0;
    static const u32 InvalidUserParam = -1;
    static const u32 SequenceBankMax = 4;
    static const u32 StreamTrackCount = 8;

    enum SoundType {
        SoundType_Invalid,
        SoundType_Sequence,
        SoundType_Stream,
        SoundType_Wave,
        SoundType_AdvancedWave,
    };

    enum DecodeMode {
        DecodeMode_Default,
        DecodeMode_Cpu,
        DecodeMode_Accelerator,
    };

    struct SoundInfo {
        FileId fileId;
        ItemId playerId;
        u8 actorPlayerId;
        u8 playerPriority;
        u8 volume;
        u8 remoteFilter;
        PanMode panMode;
        PanCurve panCurve;
        SinglePlayType singlePlayType;
        u16 singlePlayEffectiveDuration;
        bool isFrontBypass;
    };
    static_assert(sizeof(SoundInfo) == 0x1c);

    struct BankInfo {
        FileId fileId;
    };
    static_assert(sizeof(BankInfo) == 0x4);

    struct PlayerInfo {
        s32 playableSoundMax;
        u32 playerHeapSize;
    };
    static_assert(sizeof(PlayerInfo) == 0x8);

    struct SoundGroupInfo {
        ItemId startId;
        ItemId endId;
        detail::Util::Table<FileId> fileIdTable;
    };
    static_assert(sizeof(SoundGroupInfo) == 0x10);

    struct GroupInfo {
        FileId fileId;
        u32 groupFileSize;
    };
    static_assert(sizeof(GroupInfo) == 0x8);

    struct FileInfo {
        static const u32 InvalidOffset = 0xffffffff;
        static const u32 InvalidSize = 0xffffffff;

        u32 fileSize;
        u32 offsetFromFileBlockHead;
        const char* externalFilePath;

        FileInfo();
    };
    static_assert(sizeof(FileInfo) == 0x10);

    struct WaveArchiveInfo {
        u32 fileId;
        u32 waveCount;
        bool isLoadIndividual;
        u8 padding[3];
    };
    static_assert(sizeof(WaveArchiveInfo) == 0xc);

    struct SoundArchivePlayerInfo {
        s32 sequenceSoundCount;
        s32 sequenceTrackCount;
        s32 streamSoundCount;
        s32 streamTrackCount;
        s32 streamChannelCount;
        s32 waveSoundCount;
        s32 waveTrackCount;
        s32 streamBufferTimes;
        bool isAdvancedWaveSoundEnabled;
    };
    static_assert(sizeof(SoundArchivePlayerInfo) == 0x24);

    struct SequenceSoundInfo {
        u32 startOffset;
        u32 bankIds[4];
        u32 allocateTrackFlags;
        u8 channelPriority;
        bool isReleasePriorityFix;
    };
    static_assert(sizeof(SequenceSoundInfo) == 0x1c);

    enum StreamFileType {
        StreamFileType_Invalid = 0,
        StreamFileType_NwStreamBinary = 1,
        StreamFileType_Opus = 3,
    };

    struct StreamTrackInfo {
        u8 volume;
        u8 pan;
        u8 surroundPan;
        u8 flags;
        u8 mainSend;
        u8 fxSend[3];
        u8 lowPassFilterFrequency;
        u8 biquadType;
        u8 biquadValue;
        u8 channelCount;
        s8 globalChannelIndex[2];

        StreamTrackInfo();
    };
    static_assert(sizeof(StreamTrackInfo) == 0xe);

    struct StreamSoundInfo {
        u16 allocateTrackFlags;
        u16 allocateChannelCount;
        float pitch;
        u8 mainSend;
        u8 fxSend[3];
        StreamTrackInfo trackInfo[8];
        StreamFileType streamFileType;
        DecodeMode decodeMode;
        FileId prefetchFileId;
        void* streamBufferPool;

        StreamSoundInfo();
        void Setup();
    };
    static_assert(sizeof(StreamSoundInfo) == 0x90);

    struct StreamSoundInfo2 {
        bool isLoop;
        u32 loopStartFrame;
        u32 loopEndFrame;

        StreamSoundInfo2();
    };
    static_assert(sizeof(StreamSoundInfo2) == 0xc);

    struct WaveSoundInfo {
        u32 index;
        u32 allocateTrackCount;
        u8 channelPriority;
        bool isReleasePriorityFix;
    };
    static_assert(sizeof(WaveSoundInfo) == 0xc);

    struct AdvancedWaveSoundInfo {
        u32 waveArchiveId;
    };
    static_assert(sizeof(AdvancedWaveSoundInfo) == 0x4);
    
    struct Sound3DInfo {
        u32 flags;
        float decayRatio;
        u8 decayCurve;
        u8 dopplerFactor;
    };
    static_assert(sizeof(Sound3DInfo) == 0xc);

protected:
    SoundArchive();

public:
    virtual ~SoundArchive();

    bool IsAvailable() const;

    u32 GetSoundCount() const;
    u32 GetGroupCount() const; 
    u32 GetPlayerCount() const; 
    u32 GetSoundGroupCount() const; 
    u32 GetBankCount() const; 
    u32 GetWaveArchiveCount() const; 
    u32 detail_GetFileCount() const; 

    const char* GetItemLabel(ItemId id) const;
    ItemId GetItemId(const char* pStr) const;
    FileId GetItemFileId(ItemId id) const;
    FileId GetItemPrefetchFileId(ItemId id) const;

    static ItemId GetSoundIdFromIndex(u32);
    static ItemId GetSoundGroupIdFromIndex(u32);
    static ItemId GetBankIdFromIndex(u32);
    static ItemId GetPlayerIdFromIndex(u32);
    static ItemId GetWaveArchiveIdFromIndex(u32);
    static ItemId GetGroupIdFromIndex(u32);

    u32 GetSoundUserParam(ItemId soundId) const;
    bool ReadSoundUserParam(u32* pOutValue, ItemId soundId, int index) const;

    SoundType GetSoundType(ItemId soundId) const;

    bool ReadSoundInfo(SoundInfo* pOutValue, ItemId soundId) const;
    bool ReadSequenceSoundInfo(SequenceSoundInfo* pOutValue, ItemId soundId) const;
    bool ReadBankInfo(BankInfo* pOutValue, ItemId bankId) const;
    bool ReadPlayerInfo(PlayerInfo* pOutValue, ItemId playerId) const;
    bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo* pOutValue) const;
    bool ReadStreamSoundInfo(StreamSoundInfo* pOutValue, ItemId soundId) const;

    bool detail_ReadStreamSoundInfo2(ItemId soundId, StreamSoundInfo2* info) const;
    bool detail_ReadWaveSoundInfo(ItemId soundId, WaveSoundInfo* info) const;
    bool detail_ReadAdvancedWaveSoundInfo(ItemId soundId, AdvancedWaveSoundInfo* info) const;

    bool ReadSound3DInfo(Sound3DInfo* pOutValue, ItemId soundId) const;
    bool ReadWaveArchiveInfo(ItemId warcId, WaveArchiveInfo* info) const;

    bool detail_ReadSoundGroupInfo(ItemId soundGroupId, SoundGroupInfo* info) const;

    bool ReadGroupInfo(GroupInfo* pOutValue, ItemId groupId) const;

    bool detail_ReadFileInfo(FileId fileId, FileInfo* info) const;

    const detail::Util::Table<u32>* detail_GetWaveArchiveIdTable(ItemId id) const;
    virtual const void* detail_GetFileAddress(FileId fileId) const = 0;
    virtual size_t detail_GetRequiredStreamBufferSize() const = 0;

    detail::fnd::FileStream* detail_OpenFileStream(FileId fileId, void* buffer, size_t size, 
                                                   void* cacheBuffer, size_t cacheSize) const;
    const detail::Util::Table<u32>* detail_GetAttachedGroupTable(FileId fileId) const;
    
    detail::SoundArchiveParametersHook* detail_GetParametersHook() const;
    void detail_SetParametersHook(detail::SoundArchiveParametersHook* parametersHook);

    void SetExternalFileRoot(const char* extFileRoot);
    bool ReadStreamSoundFilePath(char* outFilePathBuffer, size_t filePathBufferSize, ItemId soundId) const;

    virtual void FileAccessBegin() const;
    virtual void FileAccessEnd() const;

    const char* detail_GetExternalFileFullPath(const char* externalFilePath, char* pathBuffer, 
                                               size_t bufSize) const;

    virtual bool IsAddon() const;

protected:
    void Initialize(detail::SoundArchiveFileReader* fileReader);
    void Finalize();

    virtual detail::fnd::FileStream* OpenStream(void* buffer, size_t size, 
                                                position_t begin, size_t length) const = 0;
    virtual detail::fnd::FileStream* OpenExtStream(void* buffer, size_t size, const char* extFilePath,
                                                   void* cacheBuffer, size_t cacheSize) const = 0;

    detail::fnd::FileStream* OpenExtStreamImpl(void* buffer, size_t size, const char* externalFilePath, 
                                               void* cacheBuffer, size_t cacheSize) const;
    
    static const s32 FilePathMax = 639;

private:
    friend detail::driver::StreamSoundLoader;

    detail::SoundArchiveFileReader* m_pFileReader{};
    detail::SoundArchiveParametersHook* m_pParametersHook{};
    char m_ExtFileRoot[FilePathMax];
    u32 m_FileBlockOffset;
};
static_assert(sizeof(SoundArchive) == 0x2a0);

class AddonSoundArchive : public SoundArchive {};
} // namespace nn::atk::detail