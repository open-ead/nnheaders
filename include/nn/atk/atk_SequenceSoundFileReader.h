#pragma once

#include <nn/atk/atk_SequenceSoundFile.h>

namespace nn::atk::detail {

class SequenceSoundFileReader {
public:
    static const u32 SignatureFile{0x51455346};  // FSEQ

    explicit SequenceSoundFileReader(const void* sequenceFile);

    bool IsAvailable() const { return m_pHeader != nullptr; }

    const void* GetSequenceData() const;

    bool GetOffsetByLabel(const char* label, u32* offsetPtr) const;
    const char* GetLabelByOffset(u32 offset) const;

    int GetLabelCount() const { return m_pLabelBlockBody->GetLabelCount(); }

    const char* GetLabel(int index) const { return m_pLabelBlockBody->GetLabel(index); }

private:
    const SequenceSoundFile::FileHeader* m_pHeader{};
    const SequenceSoundFile::DataBlockBody* m_pDataBlockBody{};
    const SequenceSoundFile::LabelBlockBody* m_pLabelBlockBody{};
};
static_assert(sizeof(SequenceSoundFileReader) == 0x18);

}  // namespace nn::atk::detail