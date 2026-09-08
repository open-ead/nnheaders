#include <nn/atk/atk_GroupFileReader.h>

namespace nn::atk::detail {

namespace {

const u32 SignatureInfoBlockGroup{1330007625};    // INFO
const u32 SignatureFileBlockGroup{1162627398};    // FILE
const u32 SignatureInfoexBlockGroup{1481002569};  // INFX

const u32 SupportedFileVersionGroup{0x10000};
const u32 CurrentFileVersionGroup{0x10000};

bool IsValidFileHeaderGroup(const void* groupFile) {
    const BinaryFileHeader* header{util::ConstBytePtr(groupFile).Get<BinaryFileHeader>()};

    bool isSupportedVersion{header->signature == GroupFileReader::SignatureFile &&
                            header->byteOrder == BinaryFileHeader::ValidByteOrderMark &&
                            header->version >= SupportedFileVersionGroup &&
                            header->version <= CurrentFileVersionGroup};

    return isSupportedVersion;
}

}  // anonymous namespace

GroupFileReader::GroupFileReader(const void* groupFile) {
    if (IsValidFileHeaderGroup(groupFile)) {
        auto* header{reinterpret_cast<const GroupFile::FileHeader*>(groupFile)};

        const GroupFile::InfoBlock* infoBlock{header->GetInfoBlock()};
        const GroupFile::FileBlock* fileBlock{header->GetFileBlock()};
        const GroupFile::InfoExBlock* infoExBlock{header->GetInfoExBlock()};

        if (infoBlock != nullptr && fileBlock != nullptr) {
            if (infoBlock->header.kind != SignatureInfoBlockGroup ||
                fileBlock->header.kind != SignatureFileBlockGroup)
                return;

            if (infoExBlock != nullptr) {
                if (infoExBlock->header.kind != SignatureInfoexBlockGroup)
                    return;

                m_pInfoExBlockBody = &infoExBlock->body;
            }

            m_pInfoBlockBody = &infoBlock->body;
            m_pFileBlockBody = &fileBlock->body;
        }
    }
}

}  // namespace nn::atk::detail