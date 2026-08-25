#include <nn/atk/atk_WaveSoundFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const WaveSoundFile::InfoBlock* WaveSoundFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_WaveSoundFile_InfoBlock))
            .Get<WaveSoundFile::InfoBlock>();
}
} // namespace nn::atk::detail