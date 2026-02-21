#pragma once

#include <nn/atk/detail/atk_Util.h>

namespace nn::atk::detail {
struct WaveArchiveFile {
    struct InfoBlock;
    struct FileBlock;
    struct FileHeader : BinaryFileHeader {
        Util::ReferenceWithSize toBlocks[2];

        InfoBlock* GetInfoBlock();
        u32 GetInfoBlockOffset();

        FileBlock* GetFileBlock();
        u32 GetFileBlockOffset();
    };
    static_assert(sizeof(FileHeader) == 0x2C);

    struct FileBlockBody { /* unknown structure */ };
    struct FileBlock {
        BinaryBlockHeader header;
        FileBlockBody body;
    };

    struct InfoBlockBody {
        Util::ReferenceWithSizeTable table;
    };

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };
};
} // namespace nn::atk::detail