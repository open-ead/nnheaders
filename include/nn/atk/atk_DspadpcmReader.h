#pragma once

#include <nn/atk/atk_Global.h>

namespace nn::atk::detail {
class DspadpcmReader {
public:
    DspadpcmReader();

    void Initialize(const void* dspadpcmData) {
        m_pDspadpcmData = dspadpcmData;
    }

    bool ReadWaveInfo(WaveInfo* info) const;

private:
    const void* m_pDspadpcmData {};
};
} // namespace nn::atk::detail