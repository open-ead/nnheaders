#include <nn/atk/atk_GroupFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const GroupFile::InfoBlock* GroupFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_GroupFile_InfoBlock))
            .Get<InfoBlock>();
}

const GroupFile::FileBlock* GroupFile::FileHeader::GetFileBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_GroupFile_FileBlock))
            .Get<FileBlock>();
}

const GroupFile::InfoExBlock* GroupFile::FileHeader::GetInfoExBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_GroupFile_InfoExBlock))
            .Get<InfoExBlock>();
}
} // namespace nn::atk::detail