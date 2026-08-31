#pragma once

#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {
struct WaveArchiveFile {
    static const int BlockCount {2};

    struct InfoBlock;
    struct FileBlock;
    struct FileHeader : BinaryFileHeader {
    public:
        Util::ReferenceWithSize toBlocks[BlockCount];
        
        const InfoBlock* GetInfoBlock() const;
        const FileBlock* GetFileBlock() const;
        
        u32 GetInfoBlockSize() const;
        u32 GetFileBlockSize() const;

        u32 GetInfoBlockOffset() const;
        u32 GetFileBlockOffset() const;
        
    private:
        const Util::ReferenceWithSize* GetReferenceBy(u16 typeId) const;
    };
    static_assert(sizeof(FileHeader) == 0x2c);

    struct InfoBlockBody {
        Util::Table<Util::ReferenceWithSize> table;

        u32 GetWaveFileCount() const { return table.count; }

        u32 GetSize(u32 index) const {
            return table.item[index].size;
        }

        u32 GetOffsetFromFileBlockBody(u32 index) const {
            return table.item[index].offset;
        }

        static const u32 InvalidOffset = 0xffffffff;
    };

    struct InfoBlock {
        BinaryBlockHeader header;
        InfoBlockBody body;
    };

    struct FileBlockBody {  };

    struct FileBlock {
        BinaryBlockHeader header;
        FileBlockBody body;
    };
    static_assert(sizeof(FileBlock) == 0xc);
};
} // namespace nn::atk::detail