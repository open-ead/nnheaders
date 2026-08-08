#pragma once

namespace nn::atk::detail {
struct SoundArchiveFilesHook {
    constexpr static const char* ItemTypeStreamSound = "stm";
    constexpr static const char* ItemTypeWaveSound = "wsd";
    constexpr static const char* ItemTypeSequenceSound = "seq";

    constexpr static const char* FileTypeStreamBinary = "bxstm";
    constexpr static const char* FileTypeWaveSoundBinary = "bxwsd";
    constexpr static const char* FileTypeSequenceBinary = "bxseq";
    constexpr static const char* FileTypeBankBinary = "bxbnk";
    constexpr static const char* FileTypeWaveArchiveBinary = "bxwar";
    constexpr static const char* FileTypeStreamPrefetchBinary = "bxstp";
};
} // namespace nn::atk::detail