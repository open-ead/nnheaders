#include <nn/atk/atk_BankFile.h>

#include <nn/atk/atk_ElementType.h>

namespace nn::atk::detail {
const BankFile::InfoBlock* BankFile::FileHeader::GetInfoBlock() const {
    return util::ConstBytePtr(GetBlock(ElementType_BankFile_InfoBlock))
            .Get<InfoBlock>();
}

const Util::WaveIdTable& BankFile::InfoBlockBody::GetWaveIdTable() const {
    return *util::ConstBytePtr(this).Advance(toWaveIdTable.offset)
            .Get<Util::WaveIdTable>();
}

const Util::ReferenceTable& BankFile::InfoBlockBody::GetInstrumentReferenceTable() const {
    return *util::ConstBytePtr(this).Advance(toInstrumentReferenceTable.offset)
            .Get<Util::ReferenceTable>();
}
} // namespace nn::atk::detail