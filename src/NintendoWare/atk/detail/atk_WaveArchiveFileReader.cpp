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

WaveArchiveFileReader::WaveArchiveFileReader() {
    m_IsInitialized = false;
    m_pInfoBlockBody = nullptr;
    m_pLoadTable = nullptr;
    m_pHeader = nullptr;
}

}  // namespace nn::atk::detail