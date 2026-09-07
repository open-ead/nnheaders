#include <nn/atk/atk_WaveArchiveFileReader.h>

namespace nn::atk::detail {

namespace {

const u32 SupportedFileVersionWar{0x10000};
const u32 CurrentFileVersionWar{0x10000};

bool IsValidFileHeaderWar(const void* waveArchiveData) {
    const BinaryFileHeader& header{*util::ConstBytePtr(waveArchiveData).Get<BinaryFileHeader>()};

    bool isSupportedVersion{header.signature == WaveArchiveFileReader::SignatureFile &&
                            header.byteOrder == BinaryFileHeader::ValidByteOrderMark &&
                            header.version >= SupportedFileVersionWar &&
                            header.version <= CurrentFileVersionWar};

    return isSupportedVersion;
}

}  // anonymous namespace

WaveArchiveFileReader::WaveArchiveFileReader() = default;

WaveArchiveFileReader::WaveArchiveFileReader(const void* pWaveArchiveFile, bool isIndividual) {
    Initialize(pWaveArchiveFile, isIndividual);
}

void WaveArchiveFileReader::Initialize(const void* pWaveArchiveFile, bool isIndividual) {
    if (pWaveArchiveFile == nullptr || !IsValidFileHeaderWar(pWaveArchiveFile))
        return;

    m_pHeader = reinterpret_cast<const WaveArchiveFile::FileHeader*>(pWaveArchiveFile);

    m_pInfoBlockBody = &m_pHeader->GetInfoBlock()->body;
    m_IsInitialized = true;

    m_pLoadTable = nullptr;

    if (!isIndividual || !HasIndividualLoadTable())
        return;

    m_pLoadTable = util::BytePtr(const_cast<void*>(pWaveArchiveFile))
                       .Advance(m_pHeader->GetFileBlockOffset() +
                                sizeof(WaveArchiveFileReader::SignatureWarcTable))
                       .Get<IndividualLoadTable>();
}

bool WaveArchiveFileReader::HasIndividualLoadTable() const {
    if (!m_IsInitialized)
        return false;

    const u32* signature{util::ConstBytePtr(m_pHeader, m_pHeader->GetFileBlockOffset()).Get<u32>()};

    return *signature == SignatureWarcTable;
}

}  // namespace nn::atk::detail