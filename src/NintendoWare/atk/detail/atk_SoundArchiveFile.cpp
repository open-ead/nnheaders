#include <nn/atk/atk_SoundArchiveFile.h>

#include <cstring>

namespace nn::atk::detail {
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

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetSoundInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toSoundInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}

const Util::ReferenceTable& SoundArchiveFile::InfoBlockBody::GetBankInfoReferenceTable() const {
    return *util::ConstBytePtr(this, toBankInfoReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}
} // namespace nn::atk::detail