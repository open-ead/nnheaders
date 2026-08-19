#pragma once

#include <nn/types.h>
#include <nn/atk/fnd/io/atkfnd_FileStream.h>

namespace nn::atk::detail {
class SoundArchiveFilesHook {
public:
    constexpr static const char ItemTypeWaveSound[] = "wsd";
    constexpr static const char ItemTypeStreamSound[] = "stm";
    constexpr static const char ItemTypeSequenceSound[] = "seq";

    constexpr static const char FileTypeStreamBinary[] = "bxstm";
    constexpr static const char FileTypeWaveSoundBinary[] = "bxwsd";
    constexpr static const char FileTypeSequenceBinary[] = "bxseq";
    constexpr static const char FileTypeBankBinary[] = "bxbnk";
    constexpr static const char FileTypeWaveArchiveBinary[] = "bxwar";
    constexpr static const char FileTypeStreamPrefetchBinary[] = "bxstp";

    bool GetIsEnable() const;
    void SetIsEnable(bool value);

    bool IsTargetItem(const char* itemLabel);

    void Lock();
    void Unlock();

    fnd::FileStream* OpenFile(void* buffer, size_t bufferLength, void* cacheBuffer, size_t cacheBufferLength, const char* itemLabel, const char* fileType);

    const void* GetFileAddress(const char* itemLabel, const char* itemType, const char* fileType, u32 fileIndex);

private:
    bool m_IsEnable;
};
} // namespace nn::atk::detail