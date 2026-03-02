#pragma once

#include <nn/atk/detail/atk_Config.h>
#include <nn/atk/detail/util/atk_Util.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk {
namespace detail{
class SoundArchiveFileReader;
struct SoundArchiveParametersHook;
struct SoundArchiveFilesHook;
} // namespace nn::atk::detail

class SoundArchive {
public:
    using ItemId = u32;
    using FileId = ItemId;
    using StringId = ItemId;

    constexpr static ItemId InvalidId = -1;

    constexpr static u32 UserParamIndexMax = 3;
    constexpr static u32 ResultInvalidSoundId = 0;
    constexpr static u32 InvalidUserParam = -1;
    constexpr static u32 SequenceBankMax = 4;
    constexpr static u32 StreamTrackCount = 8;

    constexpr static s32 FilePathMax = 639;

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
        u32 fileSize;
        u32 offsetFromFileBlockHead;
        char* externalFilePath;
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
    };
    static_assert(sizeof(StreamTrackInfo) == 0xe);

    struct StreamSoundInfo {
        void Setup();

        u16 allocateTrackFlags;
        u16 allocateChannelCount;
        f32 pitch;
        u8 mainSend;
        u8 fxSend[3];
        StreamTrackInfo trackInfo[8];
        StreamFileType streamFileType;
        DecodeMode decodeMode;
        FileId prefetchFileId;
        void* streamBufferPool;
    };
    static_assert(sizeof(StreamSoundInfo) == 0x90);

    struct StreamSoundInfo2 {
        bool isLoop;
        u32 loopStartFrame;
        u32 loopEndFrame;
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
        f32 decayRatio;
        u8 decayCurve;
        u8 dopplerFactor;
    };
    static_assert(sizeof(Sound3DInfo) == 0xc);

    SoundArchive();

    virtual ~SoundArchive();

    bool IsAvailable() const;

    void Initialize(detail::SoundArchiveFileReader* reader);
    void Finalize();

    s32 GetSoundCount() const;
    s32 GetPlayerCount() const; 
    s32 GetSoundGroupCount() const; 
    s32 GetGroupCount() const; 
    s32 GetBankCount() const; 
    s32 GetWaveArchiveCount() const; 

    s32 detail_GetFileCount() const; 

    char* GetItemLabel(ItemId id) const;
    FileId GetItemFileId(ItemId id) const;
    FileId GetItemPrefetchFileId(ItemId id) const;

    void* GetSoundUserParam(u32) const;

    bool ReadSoundUserParam(u32*, u32, s32) const;

    u32 GetSoundType(u32) const;

    bool ReadSoundInfo(SoundInfo* info, u32) const;
    bool ReadSequenceSoundInfo(SequenceSoundInfo* info, u32) const;
    bool ReadStreamSoundInfo(StreamSoundInfo* info, u32) const;

    bool detail_ReadStreamSoundInfo2(u32, StreamSoundInfo2*) const;
    bool detail_ReadWaveSoundInfo(u32, WaveSoundInfo*) const;
    bool detail_ReadAdvancedWaveSoundInfo(u32, AdvancedWaveSoundInfo*) const;

    bool ReadPlayerInfo(PlayerInfo*, u32) const;
    bool ReadSoundArchivePlayerInfo(SoundArchivePlayerInfo*) const;
    bool ReadSound3DInfo(Sound3DInfo*, u32) const;
    bool ReadBankInfo(BankInfo*, u32) const;
    bool ReadWaveArchiveInfo(u32, WaveArchiveInfo*) const;

    bool detail_ReadSoundGroupInfo(u32, SoundGroupInfo*) const;

    bool ReadGroupInfo(GroupInfo*, u32) const;

    bool detail_ReadFileInfo(u32, FileInfo*) const;

    detail::Util::Table<s32>* detail_GetWaveArchiveIdTable(ItemId id);

    bool detail_OpenFileStream(FileId id, void*, size_t, void*, size_t) const;

    void OpenExtStreamImpl(void*, size_t, const char*, void*, size_t) const;

    char* detail_GetExternalFileFullPath(const char*, char*, size_t) const;

    void SetExternalFileRoot(const char*);

    bool ReadStreamSoundFilePath(char*, size_t, u32);

    void* detail_GetAttachedGroupTable(u32) const;

    virtual void* detail_GetFileAddress(ItemId itemId) = 0;
    virtual size_t detail_GetRequiredStreamBufferSize() const = 0;
    
    virtual void FileAccessBegin() const;
    virtual void FileAccessEnd() const;

    virtual bool IsAddon();

    virtual detail::fnd::FileStream* OpenStream(void* buffer, size_t size, 
                                                position_t begin, size_t length) const = 0;

    virtual detail::fnd::FileStream* OpenExtStream(void* buffer, size_t size, const char* extFilePath,
                                                   void* cacheBuffer, size_t cacheSize) const = 0;

private:
    detail::SoundArchiveFileReader* m_pFileReader{};
    detail::SoundArchiveParametersHook* m_pParametersHook{};
    char m_ExtFileRoot[FilePathMax];
    u32 m_FileBlockOffset;
};
static_assert(sizeof(SoundArchive) == 0x2a0);
} // namespace nn::atk::detail