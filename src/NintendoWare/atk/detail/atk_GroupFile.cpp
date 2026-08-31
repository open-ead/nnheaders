#include <nn/atk/atk_GroupFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const GroupFile::InfoBlock* GroupFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_GroupFile_InfoBlock))
            .Get<InfoBlock>();
}
} // namespace nn::atk::detail