#include <nn/atk/atk_SoundArchiveFile.h>

#include <cstring>
#include "nn/util/util_BytePtr.h"

namespace nn::atk::detail {
namespace {
const u32 DefaultStringId {0xffffffff}; // 26
const PanMode DefaultPanMode {PanMode_Dual};
const PanCurve DefaultPanCurve {PanCurve_Sqrt};
const SinglePlayType DefaultSinglePlayType {SinglePlayType_None};
const u16 DefaultSinglePlayEffectiveDuration {0xffff};
const u8 DefaultPlayerPriority {64}; // 31
const u8 DefaultChannelPriority {64};
const u8 DefaultActorPlayerId {0}; // 33
const u8 DefaultIsReleasePriorityFix {0};
const bool DefaultIsFrontBypass {false};
const u32 DefaultUserParam {0xffffffff}; // 36
const u32 DefaultSeqStartOffset {0};
const u32 DefaultWarcWaveCount {0};
const u32 DefaultPlayerHeapSize {0}; // 39

enum SoundInfoBitFlag { // 44
    SoundInfoBitFlag_StringId           = 0,
    SoundInfoBitFlag_PanParam           = 1,
    SoundInfoBitFlag_PlayerParam        = 2,
    SoundInfoBitFlag_SinglePlayParam    = 3,

    SoundInfoBitFlag_OffsetTo3dParam    = 8,
    SoundInfoBitFlag_OffsetToSendParam  = 9,
    SoundInfoBitFlag_OffsetToModParam   = 10,
    SoundInfoBitFlag_OffsetToRvlParam   = 16,
    SoundInfoBitFlag_OffsetToCtrParam   = 17,
    SoundInfoBitFlag_OffsetToCafeParam  = 18,

    SoundInfoBitFlag_UserParam3         = 28,
    SoundInfoBitFlag_UserParam2         = 29,
    SoundInfoBitFlag_UserParam1         = 30,
    SoundInfoBitFlag_UserParam          = 31,
};

const int UserParamIndex[4] {}; // 65

enum WaveSoundInfoBitFlag { // 79
    WaveSoundInfoBitFlag_Priority = 0,
};

enum SequenceSoundInfoBitFlag { // 84
    SequenceSoundInfoBitFlag_StartOffset    = 0,
    SequenceSoundInfoBitFlag_Priority       = 1,
};

enum BankInfoBitFlag { // 90
    BankInfoBitFlag_StringId = 0,
};

enum PlayerInfoBitFlag { // 95
    PlayerInfoBitFlag_StringId  = 0,
    PlayerInfoBitFlag_HeapSize  = 1,
};

enum SoundGroupInfoBitFlag { // 101
    SoundGroupInfoBitFlag_StringId = 0,
};

enum GroupInfoBitFlag { // 106
    GroupInfoBitFlag_StringId = 0,
};

enum WaveArchiveInfoBitFlag { // 111
    WaveArchiveInfoBitFlag_StringId     = 0,
    WaveArchiveInfoBitFlag_WaveCount    = 1,
};
} // anonymous namespace

const Util::ReferenceWithSize* SoundArchiveFile::FileHeader::GetReferenceBy(u16 typeId) const {
    for (int i {0}; i < BlockCount; ++i) {
        if (toBlocks[i].typeId == typeId)
            return &toBlocks[i];
    }

    return nullptr;
}

u32 SoundArchiveFile::FileHeader::GetStringBlockSize() const {
    return GetReferenceBy(ElementType_SoundArchiveFile_StringBlock)->size;
}

u32 SoundArchiveFile::FileHeader::GetInfoBlockSize() const {
    return GetReferenceBy(ElementType_SoundArchiveFile_InfoBlock)->size;
}

u32 SoundArchiveFile::FileHeader::GetFileBlockSize() const {
    return GetReferenceBy(ElementType_SoundArchiveFile_FileBlock)->size;
}

int SoundArchiveFile::FileHeader::GetStringBlockOffset() const {
    return GetReferenceBy(ElementType_SoundArchiveFile_StringBlock)->offset;
}

int SoundArchiveFile::FileHeader::GetInfoBlockOffset() const {
    return GetReferenceBy(ElementType_SoundArchiveFile_InfoBlock)->offset;
}

int SoundArchiveFile::FileHeader::GetFileBlockOffset() const {
    return GetReferenceBy(ElementType_SoundArchiveFile_FileBlock)->offset;
}

const void* SoundArchiveFile::StringBlockBody::GetSection(Sections section) const {
    if (section > Sections_Max)
        return nullptr;

    return util::ConstBytePtr(this, toSection[section].offset).Get();
}

const char* SoundArchiveFile::StringBlockBody::GetString(SoundArchive::StringId stringId) const {
    if (stringId == SoundArchive::InvalidId)
        return nullptr;

    const StringTable* table {GetStringTable()};
    if (table == nullptr)
        return nullptr;
    
    return table->GetString(stringId);
}

void SoundArchiveFile::StringBlockBody::DumpTree() const {
    // TODO: Empty only in release builds
}

u32 SoundArchiveFile::StringBlockBody::GetItemIdImpl(Sections section, const char* str) const {
    const PatriciaTree* tree {GetPatriciaTree(section)};
    const PatriciaTree::NodeData* nodeData {tree->GetNodeDataBy(str)};

    if (nodeData == nullptr)
        return SoundArchive::InvalidId;

    const char* nodeDataStr {GetString(nodeData->stringId)};
    if (std::strcmp(str, nodeDataStr) != 0)
        return SoundArchive::InvalidId;

    return nodeData->itemId;
}

const SoundArchiveFile::PatriciaTree::NodeData* SoundArchiveFile::PatriciaTree::GetNodeDataBy(const char* str, size_t len) const {
    if (rootIdx >= nodeTable.count)
        return nullptr;

    const Node* node = &nodeTable.item[rootIdx];
    if (len == 0)
        len = std::strlen(str);

    while ((node->flags & Node::FlagLeaf) == 0) {
        const int pos = node->bit >> 3;
        const int bit = node->bit & 7;
        u32 nodeIdx;

        if (pos < static_cast<int>(len) && str[pos] & (1 << (7 - bit)))
            nodeIdx = node->rightIdx;
        else
            nodeIdx = node->leftIdx;

        node = &nodeTable.item[nodeIdx];
    }

    return &node->nodeData;
}

const SoundArchiveFile::SoundInfo* SoundArchiveFile::InfoBlockBody::GetSoundInfo(SoundArchive::ItemId itemId) const {
    if (Util::GetItemType(itemId) != ItemType_Sound)
        return nullptr;

    u32 index {Util::GetItemIndex(itemId)};
    const Util::ReferenceTable& table {GetSoundInfoReferenceTable()};

    if (index >= table.count)
        return nullptr;

    return util::ConstBytePtr(table.GetReferedItem(index)).Get<SoundInfo>();
}

const SoundArchiveFile::BankInfo* SoundArchiveFile::InfoBlockBody::GetBankInfo(SoundArchive::ItemId itemId) const {
    if (Util::GetItemType(itemId) != ItemType_Bank)
        return nullptr;

    u32 index {Util::GetItemIndex(itemId)};
    const Util::ReferenceTable& table {GetBankInfoReferenceTable()};

    if (index >= table.count)
        return nullptr;

    return util::ConstBytePtr(table.GetReferedItem(index)).Get<BankInfo>();
}

const SoundArchiveFile::PlayerInfo* SoundArchiveFile::InfoBlockBody::GetPlayerInfo(SoundArchive::ItemId itemId) const {
    if (Util::GetItemType(itemId) != ItemType_Player)
        return nullptr;

    u32 index {Util::GetItemIndex(itemId)};
    const Util::ReferenceTable& table {GetPlayerInfoReferenceTable()};

    if (index >= table.count)
        return nullptr;

    return util::ConstBytePtr(table.GetReferedItem(index)).Get<PlayerInfo>();
}

const SoundArchiveFile::SoundGroupInfo* SoundArchiveFile::InfoBlockBody::GetSoundGroupInfo(SoundArchive::ItemId itemId) const {
    if (Util::GetItemType(itemId) != ItemType_SoundGroup)
        return nullptr;

    u32 index {Util::GetItemIndex(itemId)};
    const Util::ReferenceTable& table {GetSoundGroupInfoReferenceTable()};

    if (index >= table.count)
        return nullptr;

    return util::ConstBytePtr(table.GetReferedItem(index)).Get<SoundGroupInfo>();
}

const SoundArchiveFile::GroupInfo* SoundArchiveFile::InfoBlockBody::GetGroupInfo(SoundArchive::ItemId itemId) const {
    if (Util::GetItemType(itemId) != ItemType_Group)
        return nullptr;

    u32 index {Util::GetItemIndex(itemId)};
    const Util::ReferenceTable& table {GetGroupInfoReferenceTable()};

    if (index >= table.count)
        return nullptr;

    return util::ConstBytePtr(table.GetReferedItem(index)).Get<GroupInfo>();
}

const SoundArchiveFile::WaveArchiveInfo* SoundArchiveFile::InfoBlockBody::GetWaveArchiveInfo(SoundArchive::ItemId itemId) const {
    if (Util::GetItemType(itemId) != ItemType_WaveArchive)
        return nullptr;

    u32 index {Util::GetItemIndex(itemId)};
    const Util::ReferenceTable& table {GetWaveArchiveInfoReferenceTable()};

    if (index >= table.count)
        return nullptr;

    return util::ConstBytePtr(table.GetReferedItem(index)).Get<WaveArchiveInfo>();
}

const SoundArchiveFile::FileInfo* SoundArchiveFile::InfoBlockBody::GetFileInfo(SoundArchive::FileId itemId) const {
    u32 index {Util::GetItemIndex(itemId)};
    const Util::ReferenceTable& table {GetFileInfoReferenceTable()};

    if (index >= table.count)
        return nullptr;

    return util::ConstBytePtr(table.GetReferedItem(index)).Get<FileInfo>();
}

const SoundArchiveFile::SoundArchivePlayerInfo* SoundArchiveFile::InfoBlockBody::GetSoundArchivePlayerInfo() const {
    return util::ConstBytePtr(this, toSoundArchivePlayerInfo.offset)
            .Get<SoundArchivePlayerInfo>();
}

SoundArchive::FileId SoundArchiveFile::InfoBlockBody::GetItemFileId(SoundArchive::ItemId id) const {
    SoundArchive::FileId fileId {SoundArchive::InvalidId};

    switch (Util::GetItemType(id)) {
    case ItemType_Sound: {
        const SoundInfo* info {GetSoundInfo(id)};
        if (info != nullptr)
            fileId = info->fileId;
        break;
    }
    case ItemType_Bank: {
        const BankInfo* info {GetBankInfo(id)};
        if (info != nullptr)
            fileId = info->fileId;
        break;
    }
    case ItemType_WaveArchive: {
        const WaveArchiveInfo* info {GetWaveArchiveInfo(id)};
        if (info != nullptr)
            fileId = info->fileId;
        break;
    }
    case ItemType_Group: {
        const GroupInfo* info {GetGroupInfo(id)};
        if (info != nullptr)
            fileId = info->fileId;
        break;
    }
    case ItemType_SoundGroup: {
        const SoundGroupInfo* info {GetSoundGroupInfo(id)};
        if (info != nullptr) {
            SoundArchive::ItemId soundId {info->startId};
            const SoundInfo* soundInfo {GetSoundInfo(soundId)};
            if (soundInfo != nullptr)
                fileId = soundInfo->fileId;
        }
        break;
    }
    case ItemType_Player:
        break;
    }

    return fileId;
}

SoundArchive::FileId SoundArchiveFile::InfoBlockBody::GetItemPrefetchFileId(SoundArchive::ItemId id) const {
    SoundArchive::FileId fileId {SoundArchive::InvalidId};

    const SoundInfo* info {GetSoundInfo(id)};

    if (info != nullptr && info->GetSoundType() == SoundArchive::SoundType_Stream) {
        const SoundArchiveFile::StreamSoundInfo& streamSoundInfo {info->GetStreamSoundInfo()};
        fileId = streamSoundInfo.prefetchFileId;
    }

    return fileId;
}

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetSoundInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toSoundInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetBankInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toBankInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetPlayerInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toPlayerInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetSoundGroupInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toSoundGroupInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetWaveArchiveInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toWaveArchiveInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetGroupInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toGroupInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetFileInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toFileInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

SoundArchive::SoundType SoundArchiveFile::SoundInfo::GetSoundType() const {
    switch (toDetailSoundInfo.typeId) {
    case ElementType_SoundArchiveFile_SequenceSoundInfo:
        return SoundArchive::SoundType_Sequence;
    
    case ElementType_SoundArchiveFile_StreamSoundInfo:
        return SoundArchive::SoundType_Stream;

    case ElementType_SoundArchiveFile_WaveSoundInfo:
        return SoundArchive::SoundType_Wave;

    default:
        return SoundArchive::SoundType_Invalid;
    }
}

const SoundArchiveFile::StreamSoundInfo& SoundArchiveFile::SoundInfo::GetStreamSoundInfo() const {
    return *util::ConstBytePtr(this, toDetailSoundInfo.offset).Get<StreamSoundInfo>();
}
} // namespace nn::atk::detail