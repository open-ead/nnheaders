#pragma once

namespace nn::audio {
struct SinkInfo {};

struct DeviceSinkType {
    struct DownMixParameter {
        float coeff[16];
    };
    static_assert(sizeof(DownMixParameter) == 0x40);

    SinkInfo* _handle;
};
static_assert(sizeof(DeviceSinkType) == 0x8);

struct CircularBufferSinkType {
    SinkInfo* _handle;
};
static_assert(sizeof(CircularBufferSinkType) == 0x8);
} // namespace nn::audio