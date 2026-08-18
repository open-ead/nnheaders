#pragma once

#include <nn/atk/atk_Util.h>

namespace nn::atk::detail {
struct SequenceSoundFile {
    struct DataBlock;
    struct LabelBlock;
    struct FileHeader : Util::SoundFileHeader {
        const DataBlock* GetDataBlock() const;
        const LabelBlock* GetLabelBlock() const;
    };

    struct DataBlockBody {
        u8 sequenceData[1];

        const void* GetSequenceData() const;
    };

    struct DataBlock {
        BinaryBlockHeader header;
        DataBlockBody body;
    };

    struct LabelInfo;
    struct LabelBlockBody {
        Util::ReferenceTable labelInfoReferenceTable;
        
        int GetLabelCount() const { return labelInfoReferenceTable.count; };
        const LabelInfo* GetLabelInfo(int index) const;
        const char* GetLabel(int index) const;
        const char* GetLabelByOffset(u32 offset) const;
        
        bool GetOffset(int index, u32* offsetPtr) const;
        bool GetOffsetByLabel(const char* label, u32* offsetPtr) const;
    };

    struct LabelBlock {
        BinaryBlockHeader header;
        LabelBlockBody body;
    };

    struct LabelInfo {
        Util::Reference referToSequenceData;
        u32 labelStringLength;
        char label[1];
    };
    static_assert(sizeof(LabelInfo) == 0x10);
};
} // namespace nn::atk::detail