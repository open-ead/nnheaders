#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk::detail {
struct SoundArchiveParametersHook;

class SoundArchiveFile {
public:
    constexpr static u32 BlockCount = 3;

    struct FileHeader : BinaryFileHeader {

        Util::ReferenceWithSize* GetReferenceBy(u16) const;

        u32 GetStringBlockSize() const;
        u32 GetInfoBlockSize() const;
        u32 GetFileBlockSize() const;

        s32 GetStringBlockOffset() const;
        s32 GetInfoBlockOffset() const;
        s32 GetFileBlockOffset() const;

        Util::ReferenceWithSize toBlocks[BlockCount];
    };
    static_assert(sizeof(FileHeader) == 0x38);

    struct StringBlockBody {
        enum Sections {
            Sections_StringTable,
            Sections_PatriciaTree,
            Sections_Max,
        };

        void* GetSection(Sections section) const;
        char* GetString(u32) const;
        u32 GetItemIdImpl(Sections section, char* str) const;

        void DumpTree() const;

        Util::Reference toSection[1];
    };

    struct StringBlock {
        BinaryBlockHeader header;
        StringBlockBody body;
    };

    struct PatriciaTree {
        struct NodeData {
            u32 stringId;
            u32 itemId;
        };
        static_assert(sizeof(NodeData) == 0x8);

        struct Node {
            constexpr static u32 FlagLeaf = 1;

            u16 flags;
            u16 bit;
            u32 leftIdx;
            u32 rightIdx;
            NodeData nodeData;
        };
        static_assert(sizeof(Node) == 0x14);

        void* GetNodeDataBy(const char*, u64);

        u32 rootIdx;
        Util::Table<Node> nodeTable;
    };

    struct SoundInfo;
    struct BankInfo;
    struct PlayerInfo;
    struct SoundGroupInfo;
    struct GroupInfo;
    struct WaveArchiveInfo;
    struct FileInfo;
    struct SoundArchivePlayerInfo;
    struct InfoBlockBody {

        SoundInfo* GetSoundInfo(SoundArchive::ItemId itemId) const;
        Util::ReferenceTable* GetSoundInfoReferenceTable() const;

        BankInfo* GetBankInfo(SoundArchive::ItemId itemId) const;
        Util::ReferenceTable* GetBankInfoReferenceTable() const;

        PlayerInfo* GetPlayerInfo(SoundArchive::ItemId itemId) const;
        Util::ReferenceTable* GetPlayerInfoReferenceTable() const; 

        SoundGroupInfo* GetSoundGroupInfo(SoundArchive::ItemId itemId) const;
        Util::ReferenceTable* GetSoundGroupInfoReferenceTable() const; 

        GroupInfo* GetGroupInfo(SoundArchive::ItemId itemId) const;
        Util::ReferenceTable* GetGroupInfoReferenceTable() const; 

        WaveArchiveInfo* GetWaveArchiveInfo(SoundArchive::ItemId itemId) const;
        Util::ReferenceTable* GetWaveArchiveInfoReferenceTable() const;

        FileInfo* GetFileInfo(SoundArchive::FileId itemId) const;
        Util::ReferenceTable* GetFileInfoReferenceTable() const;

        SoundArchive::FileId GetItemFileId(SoundArchive::ItemId id) const;
        SoundArchive::FileId GetItemPrefetchFileId(SoundArchive::ItemId id) const;
        SoundArchive::StringId GetItemStringId(SoundArchive::ItemId id) const;

        SoundArchivePlayerInfo* GetSoundArchivePlayerInfo() const;

        Util::Reference toSoundInfoReferenceTable;
        Util::Reference toSoundGroupInfoReferenceTable;
        Util::Reference toBankInfoReferenceTable;
        Util::Reference toWaveArchiveInfoReferenceTable;
        Util::Reference toGroupInfoReferenceTable;
        Util::Reference toPlayerInfoReferenceTable;
        Util::Reference toFileInfoReferenceTable;
        Util::Reference toSoundArchivePlayerInfo;
    };
    static_assert(sizeof(InfoBlockBody) == 0x40);

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };
    static_assert(sizeof(InfoBlock) == 0x48);

    struct StreamSoundInfo;
    struct WaveSoundInfo;
    struct AdvancedWaveSoundInfo;
    struct SequenceSoundInfo;
    struct Sound3DInfo;
    struct SoundInfo {

        SoundArchive::SoundType GetSoundType() const;
        StreamSoundInfo* GetStreamSoundInfo() const;

        SoundArchive::StringId GetStringId() const;

        WaveSoundInfo* GetWaveSoundInfo() const; 
        AdvancedWaveSoundInfo* GetAdvancedWaveSoundInfo() const;
        SequenceSoundInfo* GetSequenceSoundInfo() const;
        Sound3DInfo* GetSound3DInfo() const;

        PanMode GetPanMode() const;
        PanCurve GetPanCurve() const;
        SinglePlayType GetSinglePlayType() const;
        u16 GetSinglePlayEffectiveDuration() const;
        u8 GetPlayerPriority() const;
        u8 GetActorPlayerId() const;
        u32 GetUserParam() const;

        bool ReadUserParam(u32*, s32) const;

        bool IsFrontBypass() const;

        u32 fileId;
        u32 playerId;
        u8 volume;
        u8 remoteFilter;
        u8 padding[2];
        Util::Reference toDetailSoundInfo;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(SoundInfo) == 0x18);

    struct StreamTrackInfo {
        u8 volume;
        u8 pan;
        u8 span;
        u8 flags;
        Util::Reference toGlobalChannelIndexTable;
        Util::Reference toSendValue;
        u8 lpfFreq;
        u8 biquadType;
        u8 biquadValue;
        u8 padding[1];
    };
    static_assert(sizeof(StreamTrackInfo) == 0x18);

    struct StreamTrackInfoTable {
        Util::ReferenceTable table;
    };

    struct StreamSoundExtension {
        u32 streamTypeInfo;
        u32 loopStartFrame;
        u32 loopEndFrame;
    };
    static_assert(sizeof(StreamSoundExtension) == 0xc);

    struct SendValue {
        u8 mainSend;
        u8 fxSend[3];
    };
    static_assert(sizeof(SendValue) == 0x4);

    struct StreamSoundInfo {

        StreamTrackInfoTable* GetTrackInfoTable() const;
        StreamSoundExtension* GetStreamSoundExtension() const;
        SendValue* GetSendValue() const;

        u16 allocateTrackFlags;
        u16 allocateChannelCount;
        Util::Reference toTrackInfoTable;
        f32 pitch;
        Util::Reference toSendValue;
        Util::Reference toStreamSoundExtension;
        u32 prefetchFileId;
    };
    static_assert(sizeof(StreamSoundInfo) == 0x24);

    struct WaveSoundInfo {

        u8 GetChannelPriority() const;
        u8 GetIsReleasePriorityFix() const;

        u32 index;
        u32 allocateTrackCount;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(WaveSoundInfo) == 0xc);

    struct AdvancedWaveSoundInfo {
        u32 waveArchiveId;
    };
    static_assert(sizeof(AdvancedWaveSoundInfo) == 0x4);

    struct SequenceSoundInfo {
        
        Util::ReferenceTable* GetBankIdTable() const;
        void GetBankIds(u32* bankIds) const;
        u32 GetStartOffset() const;
        u8 GetChannelPriority() const;
        
        bool IsReleasePriorityFix() const;

        Util::Reference toBankIdTable;
        u32 allocateTrackFlags;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(SequenceSoundInfo) == 0x10);

    struct Sound3DInfo {
        u32 flags;
        f32 decayRatio;
        u8 decayCurve;
        u8 dopplerFactor;
        u8 padding[2];
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(Sound3DInfo) == 0x10);

    struct BankInfo {
        
        SoundArchive::StringId GetStringId() const;

        u32 fileId;
        Util::Reference toWaveArchiveItemIdTable;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(BankInfo) == 0x10);

    struct PlayerInfo {

        SoundArchive::StringId GetStringId() const;
        u32 GetPlayerHeapSize() const;

        u32 playableSoundMax;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(PlayerInfo) == 0x8);

    struct SoundGroupInfo {
        
        SoundArchive::StringId GetStringId() const;

        u32 startId;
        u32 endId;
        Util::Reference toFileIdTable;
        Util::Reference toDetailSoundGroupInfo;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(SoundGroupInfo) == 0x1c);

    struct GroupInfo {

        SoundArchive::StringId GetStringId() const;

        u32 fileId;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(GroupInfo) == 0x8);

    struct WaveArchiveInfo {

        SoundArchive::StringId GetStringId() const;
        u32 GetWaveCount() const;

        u32 fileId;
        bool isLoadIndividual;
        u8 padding[3];
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(WaveArchiveInfo) == 0xc);

    struct SoundArchivePlayerInfo {
        u16 sequenceSoundCount;
        u16 sequenceTrackCount;
        u16 streamSoundCount;
        u16 streamTrackCount;
        u16 streamChannelCount;
        u16 waveSoundCount;
        u16 waveTrackCount;
        u8 streamBufferTimes;
        u8 developFlags;
        u32 options;
    };
    static_assert(sizeof(SoundArchivePlayerInfo) == 0x14);

    struct WaveSoundGroupInfo {
        Util::Reference toWaveArchiveItemIdTable;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(WaveSoundGroupInfo) == 0xc);

    enum FileLocationType {
        FileLocationType_Internal,
        FileLocationType_External,
        FileLocationType_None,
    };

    struct InternalFileInfo {
        constexpr static s32 InvalidOffset = -1;
        constexpr static s32 InvalidSize = -1;

        Util::ReferenceWithSize toFileImageFromFileBlockBody;
        Util::Reference toAttachedGroupIdTable;
    };
    static_assert(sizeof(InternalFileInfo) == 0x14);

    struct ExternalFileInfo {
        char filePath[1];
    };

    struct FileInfo {
    
        FileLocationType GetFileLocationType() const;
        InternalFileInfo* GetInternalFileInfo() const;
        ExternalFileInfo* GetExternalFileInfo() const;

        Util::Reference toFileLocation;
        Util::BitFlag optionParameter;
    };
    static_assert(sizeof(FileInfo) == 0xc);

    struct FileBlock {};
};
} // namespace nn::atk::detail