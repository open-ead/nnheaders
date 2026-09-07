#pragma once

#include <nn/types.h>

#include <nn/atk/fnd/basis/atkfnd_Config.h>

namespace nn::atk {
using position_t = detail::fnd::position_t;

const int StreamChannelCount{16};
const u32 StreamTrackCount{8};

const int StreamDataLoadTaskMax{32};

static const size_t DataBlockSizeMarginSamples{0x480};

} // namespace nn::atk