#pragma once

#include <nn/atk/atk_ElementType.h>
#include <nn/atk/atk_StreamSoundFile.h>
#include <nn/atk/atk_SoundArchive.h>

namespace nn::atk::detail {
class SoundArchiveParametersHook;

class SoundArchiveFile {
public:
    static const int BlockCount {3};

    struct FileHeader : BinaryFileHeader {
        Util::ReferenceWithSize toBlocks[BlockCount];

        u32 GetStringBlockSize() const;
        u32 GetInfoBlockSize() const;
        u32 GetFileBlockSize() const;

        int GetStringBlockOffset() const;
        int GetInfoBlockOffset() const;
        int GetFileBlockOffset() const;

    private:
        const Util::ReferenceWithSize* GetReferenceBy(u16 typeId) const;
    };
    static_assert(sizeof(FileHeader) == 0x38);

    struct StringTable;
    struct PatriciaTree;
    struct StringBlockBody {
        enum Sections {
            Sections_StringTable = 0,
            Sections_PatriciaTree = 1,
            Sections_Max = Sections_PatriciaTree,
        };

        Util::Reference toSection[1];

        const char* GetString(SoundArchive::StringId stringId) const;
        
        u32 GetStringCount() const {
            return GetStringTable()->GetCount();
        }

        u32 GetItemId(const char* str) const {
            return GetItemIdImpl(Sections_PatriciaTree, str);
        }

        void DumpTree() const;
    
    private:
        const void* GetSection(Sections section) const;
        
        const StringTable* GetStringTable() const {
            return util::ConstBytePtr(GetSection(Sections_StringTable)).Get<StringTable>();
        }

        const PatriciaTree* GetPatriciaTree(Sections section) const {
            // TODO
        }

        u32 GetItemIdImpl(Sections section, const char* str) const;
    };

    struct PatriciaTree {
        struct NodeData {
            u32 stringId;
            u32 itemId;
        };
        static_assert(sizeof(NodeData) == 0x8);

        struct Node {
            static const u16 FlagLeaf {1 << 0};

            u16 flags;
            u16 bit;
            u32 leftIdx;
            u32 rightIdx;
            NodeData nodeData;
        };
        static_assert(sizeof(Node) == 0x14);

        u32 rootIdx;
        Util::Table<Node> nodeTable;

        const NodeData* GetNodeDataBy(const char* str, size_t len);

        void* operator[](int idx) const {
            // TODO
        }

        void* operator[](u32 idx) const {
            // TODO
        }

        void* operator[](const char* str) const {
            // TODO
        }

        void* operator()(const char* str, size_t len) const {
            // TODO
        }
        
        u32 GetDataCount() const;
        u32 GetCount() const;
    };

    struct StringBlock {
        BinaryBlockHeader header;
        StringBlockBody body;
    };

    struct StringTable {
        Util::ReferenceWithSizeTable table;

        const char* GetString(int stringId) const {
            return util::ConstBytePtr(this, table.item[stringId].offset).Get<char>();
        }

        u32 GetCount() const {
            return table.count;
        }
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
        Util::Reference toSoundInfoReferenceTable;
        Util::Reference toSoundGroupInfoReferenceTable;
        Util::Reference toBankInfoReferenceTable;
        Util::Reference toWaveArchiveInfoReferenceTable;
        Util::Reference toGroupInfoReferenceTable;
        Util::Reference toPlayerInfoReferenceTable;
        Util::Reference toFileInfoReferenceTable;
        Util::Reference toSoundArchivePlayerInfo;

        u32 GetSoundCount() const;
        u32 GetBankCount() const;
        u32 GetPlayerCount() const;
        u32 GetSoundGroupCount() const;
        u32 GetGroupCount() const;
        u32 GetWaveArchiveCount() const;
        u32 GetFileCount() const;

        const SoundInfo* GetSoundInfo(SoundArchive::ItemId itemId) const;
        const BankInfo* GetBankInfo(SoundArchive::ItemId itemId) const;
        const PlayerInfo* GetPlayerInfo(SoundArchive::ItemId itemId) const;
        const SoundGroupInfo* GetSoundGroupInfo(SoundArchive::ItemId itemId) const;
        const GroupInfo* GetGroupInfo(SoundArchive::ItemId itemId) const;
        const WaveArchiveInfo* GetWaveArchiveInfo(SoundArchive::ItemId itemId) const;
        const FileInfo* GetFileInfo(SoundArchive::FileId itemId) const;

        const SoundArchivePlayerInfo* GetSoundArchivePlayerInfo() const;

        SoundArchive::FileId GetItemFileId(SoundArchive::ItemId id) const;
        SoundArchive::StringId GetItemStringId(SoundArchive::ItemId id) const;
        SoundArchive::FileId GetItemPrefetchFileId(SoundArchive::ItemId id) const;

    private:
        const Util::ReferenceTable& GetSoundInfoReferenceTable() const;
        const Util::ReferenceTable& GetBankInfoReferenceTable() const;
        const Util::ReferenceTable& GetPlayerInfoReferenceTable() const;
        const Util::ReferenceTable& GetSoundGroupInfoReferenceTable() const;
        const Util::ReferenceTable& GetWaveArchiveInfoReferenceTable() const;
        const Util::ReferenceTable& GetGroupInfoReferenceTable() const;
        const Util::ReferenceTable& GetFileInfoReferenceTable() const;
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
        u32 fileId;
        u32 playerId;
        u8 volume;
        u8 remoteFilter;
        u8 padding[2];
        Util::Reference toDetailSoundInfo;
        Util::BitFlag optionParameter;

        SoundArchive::SoundType GetSoundType() const;
        const StreamSoundInfo& GetStreamSoundInfo() const;
        const WaveSoundInfo& GetWaveSoundInfo() const;
        const AdvancedWaveSoundInfo& GetAdvancedWaveSoundInfo() const;
        const SequenceSoundInfo& GetSequenceSoundInfo() const;
        const Sound3DInfo& GetSound3DInfo() const;

        u32 GetStringId() const;
        PanMode GetPanMode() const;
        PanCurve GetPanCurve() const;
        SinglePlayType GetSinglePlayType() const;
        u16 GetSinglePlayEffectiveDuration() const;
        u8 GetPlayerPriority() const;
        u8 GetActorPlayerId() const;
        u32 GetUserParam() const;

        bool ReadUserParam(u32* pOutValue, int index) const;

        bool IsFrontBypass() const;
    };
    static_assert(sizeof(SoundInfo) == 0x18);

    struct StreamTrackInfo;
    struct StreamTrackInfoTable {
        Util::ReferenceTable table;

        const StreamTrackInfo* GetTrackInfo(u32 index) {
            return util::ConstBytePtr(table.GetReferedItem(ElementType_SoundArchiveFile_StreamSoundTrackInfo, index))
                    .Get<StreamTrackInfo>();
        }
        
        u32 GetTrackCount() const {
            return table.count;
        }
    };

    struct SendValue {
        u8 mainSend;
        u8 fxSend[3];
    };
    static_assert(sizeof(SendValue) == 0x4);

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

        u32 GetTrackChannelCount() const {
            return GetGlobalChannelIndexTable().GetCount();
        }

        u8 GetGlobalChannelIndex(u32 index) const {
            return GetGlobalChannelIndexTable().GetGlobalIndex(index);
        }

        const SendValue& GetSendValue() const {
            return *util::ConstBytePtr(this, toSendValue.offset)
                    .Get<SendValue>();
        }

        const StreamSoundFile::GlobalChannelIndexTable& GetGlobalChannelIndexTable() const {
            return *util::ConstBytePtr(this, toGlobalChannelIndexTable.offset)
                    .Get<StreamSoundFile::GlobalChannelIndexTable>();
        }
    };
    static_assert(sizeof(StreamTrackInfo) == 0x18);

    struct StreamSoundExtension {
        u32 streamTypeInfo;
        u32 loopStartFrame;
        u32 loopEndFrame;

        SoundArchive::StreamFileType GetStreamFileType() const {
            switch (Util::DivideBy8bit(streamTypeInfo, 0)) {
            case 1:
                return SoundArchive::StreamFileType_NwStreamBinary;
            case 3:
                return SoundArchive::StreamFileType_Opus;
            default:
                return SoundArchive::StreamFileType_Invalid;
            }
        }

        bool IsLoop() const {
            return Util::DivideBy8bit(streamTypeInfo, 1) != 0;
        }

        SoundArchive::DecodeMode GetDecodeMode() const {
            switch (Util::DivideBy8bit(streamTypeInfo, 2)) {
            case 1:
                return SoundArchive::DecodeMode_Cpu;
            case 2:
                return SoundArchive::DecodeMode_Accelerator;
            default:
                return SoundArchive::DecodeMode_Default;
            }
        }
    };
    static_assert(sizeof(StreamSoundExtension) == 0xc);

    struct StreamSoundInfo {
        u16 allocateTrackFlags;
        u16 allocateChannelCount;
        Util::Reference toTrackInfoTable;
        float pitch;
        Util::Reference toSendValue;
        Util::Reference toStreamSoundExtension;
        u32 prefetchFileId;

        const StreamTrackInfoTable* GetTrackInfoTable() const;

        float GetPitch() const { return pitch; }
        const SendValue& GetSendValue() const;
        const StreamSoundExtension* GetStreamSoundExtension() const;
    };
    static_assert(sizeof(StreamSoundInfo) == 0x24);

    struct WaveSoundInfo {
        u32 index;
        u32 allocateTrackCount;
        Util::BitFlag optionParameter;

        u8 GetChannelPriority() const;
        u8 GetIsReleasePriorityFix() const;
    };
    static_assert(sizeof(WaveSoundInfo) == 0xc);

    struct AdvancedWaveSoundInfo {
        u32 waveArchiveId;
    };
    static_assert(sizeof(AdvancedWaveSoundInfo) == 0x4);

    struct SequenceSoundInfo {
        Util::Reference toBankIdTable;
        u32 allocateTrackFlags;
        Util::BitFlag optionParameter;

        void GetBankIds(u32* bankIds) const;
        u32 GetStartOffset() const;
        u8 GetChannelPriority() const;
        bool IsReleasePriorityFix() const;

        const Util::Table<u32>& GetBankIdTable() const;
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
        u32 fileId;
        Util::Reference toWaveArchiveItemIdTable;
        Util::BitFlag optionParameter;

        u32 GetStringId() const;

        const Util::Table<SoundArchive::ItemId>* GetWaveArchiveItemIdTable() const {
            return util::ConstBytePtr(this, toWaveArchiveItemIdTable.offset)
                    .Get<Util::Table<SoundArchive::ItemId>>();
        }
    };
    static_assert(sizeof(BankInfo) == 0x10);

    struct PlayerInfo {
        u32 playableSoundMax;
        Util::BitFlag optionParameter;

        u32 GetStringId() const;
        u32 GetPlayerHeapSize() const;
    };
    static_assert(sizeof(PlayerInfo) == 0x8);

    struct WaveSoundGroupInfo;
    struct SoundGroupInfo {
        u32 startId;
        u32 endId;
        Util::Reference toFileIdTable;
        Util::Reference toDetailSoundGroupInfo;
        Util::BitFlag optionParameter;

        u32 GetStringId() const;

        const Util::Table<SoundArchive::FileId>* GetFileIdTable() const {
            return util::ConstBytePtr(this, toFileIdTable.offset)
                    .Get<Util::Table<SoundArchive::FileId>>();
        }

        const WaveSoundGroupInfo* GetWaveSoundGroupInfo() const {
            return util::ConstBytePtr(this, toDetailSoundGroupInfo.offset)
                    .Get<WaveSoundGroupInfo>();
        }
    };
    static_assert(sizeof(SoundGroupInfo) == 0x1c);

    struct WaveSoundGroupInfo {
        Util::Reference toWaveArchiveItemIdTable;
        Util::BitFlag optionParameter;

        const Util::Table<SoundArchive::ItemId>* GetWaveArchiveItemIdTable() const {
            return util::ConstBytePtr(this, toWaveArchiveItemIdTable.offset)
                    .Get<Util::Table<SoundArchive::ItemId>>();
        }
    };
    static_assert(sizeof(WaveSoundGroupInfo) == 0xc);

    struct GroupInfo {
        u32 fileId;
        Util::BitFlag optionParameter;

        u32 GetStringId() const;
    };
    static_assert(sizeof(GroupInfo) == 0x8);

    struct WaveArchiveInfo {
        u32 fileId;
        bool isLoadIndividual;
        u8 padding[3];
        Util::BitFlag optionParameter;

        u32 GetStringId() const;
        u32 GetWaveCount() const;
    };
    static_assert(sizeof(WaveArchiveInfo) == 0xc);

    enum FileLocationType {
        FileLocationType_Internal,
        FileLocationType_External,
        FileLocationType_None,
    };

    struct InternalFileInfo;
    struct ExternalFileInfo;
    struct FileInfo {
        Util::Reference toFileLocation;
        Util::BitFlag optionParameter;

        FileLocationType GetFileLocationType() const;
        const InternalFileInfo* GetInternalFileInfo() const;
        const ExternalFileInfo* GetExternalFileInfo() const;
    };
    static_assert(sizeof(FileInfo) == 0xc);

    struct InternalFileInfo {
        static const u32 InvalidOffset {0xffffffff};
        static const u32 InvalidSize   {0xffffffff};

        Util::ReferenceWithSize toFileImageFromFileBlockBody;
        Util::Reference toAttachedGroupIdTable;

        u32 GetFileSize() const {
            return toFileImageFromFileBlockBody.size;
        }

        u32 GetOffsetFromFileBlockHead() const {
            return toFileImageFromFileBlockBody.offset;
        }

        const Util::Table<u32>* GetAttachedGroupTable() const {
            return util::ConstBytePtr(this, toAttachedGroupIdTable.offset)
                    .Get<Util::Table<u32>>();
        }
    };
    static_assert(sizeof(InternalFileInfo) == 0x14);

    struct ExternalFileInfo {
        char filePath[1];
    };

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

    struct FileBlock {};
};
} // namespace nn::atk::detail