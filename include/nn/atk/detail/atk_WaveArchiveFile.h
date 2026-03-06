#pragma once

#include <nn/atk/detail/util/atk_Util.h>

namespace nn::atk::detail {
struct WaveArchiveFile {
    constexpr static s32 BlockCount = 2;

    struct InfoBlock;
    struct FileBlock;
    struct FileHeader : BinaryFileHeader {
        
        InfoBlock* GetInfoBlock() const;
        u32 GetInfoBlockOffset() const;

        FileBlock* GetFileBlock() const;
        u32 GetFileBlockOffset() const;

        u32 GetInfoBlockSize() const;

        Util::Reference* GetReferenceBy(u16);

        u32 GetFileBlockSize() const;

        Util::ReferenceWithSize toBlocks[BlockCount];
    };
    static_assert(sizeof(FileHeader) == 0x2C);

    struct FileBlockBody { /* unknown structure */ };
    struct FileBlock {
        BinaryBlockHeader header;
        FileBlockBody body;
    };

    struct InfoBlockBody {
        constexpr static s32 InvalidOffset = -1;

        Util::ReferenceWithSizeTable table;
    };

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };
};
} // namespace nn::atk::detail