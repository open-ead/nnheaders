#include <nn/atk/atk_BankFileReader.h>

namespace nn::atk::detail {

namespace {
const u32 SignatureInfoBlockBank{0x4f464e49};  // INFO

const u32 SupportedFileVersionBank{0x10000};
const u32 CurrentFileVersionBank{0x10000};

bool IsValidFileHeaderBank(const void* bankFile) {
    const BinaryFileHeader* header{util::ConstBytePtr(bankFile).Get<BinaryFileHeader>()};

    bool isSupportedVersion{header->signature == BankFileReader::SignatureFile &&
                            header->byteOrder == BinaryFileHeader::ValidByteOrderMark &&
                            header->version >= SupportedFileVersionBank &&
                            header->version <= CurrentFileVersionBank};

    return isSupportedVersion;
}
}  // anonymous namespace

BankFileReader::BankFileReader() = default;

BankFileReader::BankFileReader(const void* bankFile) {
    Initialize(bankFile);
}

void BankFileReader::Initialize(const void* bankFile) {
    if (bankFile == nullptr || !IsValidFileHeaderBank(bankFile))
        return;

    m_pHeader = reinterpret_cast<const BankFile::FileHeader*>(bankFile);

    const BankFile::InfoBlock* infoBlock{m_pHeader->GetInfoBlock()};

    if (infoBlock->header.kind == SignatureInfoBlockBank) {
        m_pInfoBlockBody = &infoBlock->body;
        m_IsInitialized = true;
    }
}

void BankFileReader::Finalize() {
    if (m_IsInitialized) {
        m_IsInitialized = false;
        m_pHeader = nullptr;
        m_pInfoBlockBody = nullptr;
    }
}

}  // namespace nn::atk::detail