#pragma once

#include <nn/atk/atk_Global.h>

namespace nn::atk::detail {
class DspadpcmReader {
public:
    DspadpcmReader();

    bool ReadWaveInfo(WaveInfo* info);

private:
    void* m_pDspadpcmData;
};
} // namespace nn::atk::detail