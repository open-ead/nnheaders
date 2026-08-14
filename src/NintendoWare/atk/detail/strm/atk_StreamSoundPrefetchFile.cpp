#include <nn/atk/atk_StreamSoundPrefetchFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const StreamSoundFile::InfoBlock* StreamSoundPrefetchFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_StreamSoundFile_InfoBlock))
            .Get<StreamSoundFile::InfoBlock>();
}
} // namespace nn::atk::detail