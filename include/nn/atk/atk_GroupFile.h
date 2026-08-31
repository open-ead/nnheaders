#pragma once

#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {
struct GroupFile {
    struct InfoBlock;
    struct FileBlock;
    struct InfoExBlock;
    struct FileHeader : Util::SoundFileHeader {
        const InfoBlock* GetInfoBlock() const;
        const FileBlock* GetFileBlock() const;
        const InfoExBlock* GetInfoExBlock() const;
    };

    struct GroupItemInfo;
    struct InfoBlockBody {
        Util::ReferenceTable referenceTableOfGroupItemInfo;

        u32 GetGroupItemInfoCount() const { 
            return referenceTableOfGroupItemInfo.count; 
        }

        const GroupItemInfo* GetGroupItemInfo(u32 index) const {
            if (GetGroupItemInfoCount() <= index)
                return nullptr;

            return util::ConstBytePtr(this, referenceTableOfGroupItemInfo.item[index].offset)
                    .Get<GroupItemInfo>();
        }
    };

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    struct FileBlockBody;
    struct GroupItemInfo {
        u32 fileId;
        Util::ReferenceWithSize embeddedItemInfo;

        static const u32 OffsetForLink {0xffffffff};
        static const u32 SizeForLink   {0xffffffff};

        const void* GetFileLocation(const FileBlockBody* fileBlockBody) const {
            // TODO: check if this is int or u32 comparison
            if (static_cast<u32>(embeddedItemInfo.offset) == OffsetForLink)
                return nullptr;

            return util::ConstBytePtr(fileBlockBody, embeddedItemInfo.offset).Get();
        }
    };
    static_assert(sizeof(GroupItemInfo) == 0x10);

    struct FileBlockBody {/* empty structure */};

    struct FileBlock {
        BinaryBlockHeader header;
        FileBlockBody body;
    };

    struct GroupItemInfoEx;
    struct InfoExBlockBody {
        Util::ReferenceTable referenceTableOfGroupItemInfoEx;

        u32 GetGroupItemInfoExCount() const {
            return referenceTableOfGroupItemInfoEx.count;
        }

        const GroupItemInfoEx* GetGroupItemInfoEx(u32 index) const {
            if (GetGroupItemInfoExCount() <= index)
                return nullptr;

            return util::ConstBytePtr(this, referenceTableOfGroupItemInfoEx.item[index].offset)
                    .Get<GroupItemInfoEx>();
        }
    };

    struct InfoExBlock {
        BinaryBlockHeader header;
        InfoExBlockBody body;
    };

    struct GroupItemInfoEx {
        u32 itemId;
        u32 loadFlag;
    };
    static_assert(sizeof(GroupItemInfoEx) == 0x8);

};

} // namespace nn::atk::detail