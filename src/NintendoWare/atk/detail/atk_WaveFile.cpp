#include <nn/atk/atk_WaveFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const WaveFile::InfoBlock* WaveFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_WaveFile_InfoBlock))
        .Get<WaveFile::InfoBlock>();
}

const WaveFile::DataBlock* WaveFile::FileHeader::GetDataBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_WaveFile_DataBlock))
        .Get<WaveFile::DataBlock>();
}
} // namespace nn::atk::detail