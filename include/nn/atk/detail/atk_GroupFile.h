#pragma once

#include <nn/atk/detail/atk_Util.h>

namespace nn::atk::detail {
struct GroupFile {
    struct FileHeader : Util::SoundFileHeader {
        struct InfoBlock* GetInfoBlock();
        struct FileBlock* GetFileBlock();
        struct InfoExBlock* GetInfoExBlock();
    };

    struct InfoBlockBody {
        Util::ReferenceTable referenceTableOfGroupItemInfo;
    };

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    struct FileBlockBody {/* unknown structure */};

    struct FileBlock {
        BinaryBlockHeader header;
        FileBlockBody body;
    };

    struct InfoExBlockBody {
        Util::ReferenceTable referenceTableOfGroupItemInfoEx;
    };

    struct InfoExBlock {
        BinaryBlockHeader header;
        InfoExBlockBody body;
    };

    struct GroupItemInfo {
        constexpr static s32 OffsetForLink = -1;
        constexpr static s32 SizeForLink = -1;

        u32 fileId;
        Util::ReferenceWithSize embeddedItemInfo;
    };
    static_assert(sizeof(GroupItemInfo) == 0x10);

    struct GroupItemInfoEx {
        u32 itemId;
        u32 loadFlag;
    };
    static_assert(sizeof(GroupItemInfoEx) == 0x8);

};

} // namespace nn::atk::detail