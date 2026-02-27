#pragma once

#include <nn/atk/detail/seq/atk_SequenceSoundFile.h>

namespace nn::atk::detail {
class SequenceSoundFileReader {
public:
    constexpr static s32 SignatureFile = 51455346; // FSEQ

    explicit SequenceSoundFileReader(void* sequenceFile);

    void* GetSequenceData();

    bool GetOffsetByLabel(const char* label, u32* offsetPtr) const;
    bool GetLabelByOffset(u32 offset) const;

private:
    SequenceSoundFile::FileHeader* m_pHeader;
    SequenceSoundFile::DataBlockBody* m_pDataBlockBody;
    SequenceSoundFile::LabelBlockBody* m_pLabelBlockBody;
};
static_assert(sizeof(SequenceSoundFileReader) == 0x18);
} // namespace nn::atk::detail