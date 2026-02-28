#pragma once

namespace nn::audio {
struct MemoryPoolInfo {};

struct MemoryPoolType {
    enum State {
        State_RequestAttach,
        State_Attached,
        State_RequestDetach,
        State_Detached,
    };

    MemoryPoolInfo* _pMemoryPoolInfo;
};
static_assert(sizeof(MemoryPoolType) == 0x8);
} // namespace nn::audio
