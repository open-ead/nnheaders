#pragma once

#include <nn/atk/detail/atk_Util.h>

namespace nn::atk::detail {
struct SoundArchiveParametersHook;

class SoundArchiveFile {
public:
    struct FileHeader : BinaryFileHeader {
        Util::ReferenceWithSize toBlocks[3];

        Util::ReferenceWithSize* GetReferenceBy(u16);

        u32 GetStringBlockSize();
        u32 GetInfoBlockSize();
        u32 GetFileBlockSize();

        s32 GetStringBlockOffset();
        s32 GetInfoBlockOffset();
        s32 GetFileBlockOffset();
    };
    static_assert(sizeof(FileHeader) == 0x38);

    struct StringBlockBody {
        Util::Reference toSection[1];
    };

    struct InfoBlockBody {
        Util::Reference toSoundInfoReferenceTable;
        Util::Reference toSoundGroupInfoReferenceTable;
        Util::Reference toBankInfoReferenceTable;
        Util::Reference toWaveArchiveInfoReferenceTable;
        Util::Reference toGroupInfoReferenceTable;
        Util::Reference toPlayerInfoReferenceTable;
        Util::Reference toFileInfoReferenceTable;
        Util::Reference toSoundArchivePlayerInfo;
    };
    static_assert(sizeof(InfoBlockBody) == 0x40);
};
} // namespace nn::atk::detail