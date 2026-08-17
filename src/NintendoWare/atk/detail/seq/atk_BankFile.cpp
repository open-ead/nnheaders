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

const BankFile::Instrument* BankFile::InfoBlockBody::GetInstrument(int programNo) const {
    auto& table {GetInstrumentReferenceTable()};
    auto& ref {table.item[programNo]};

    if (ref.IsValidTypeId(ElementType_BankFile_InstrumentInfo))
        return util::ConstBytePtr(table.GetReferedItem(programNo)).Get<BankFile::Instrument>();

    return nullptr;
}
} // namespace nn::atk::detail