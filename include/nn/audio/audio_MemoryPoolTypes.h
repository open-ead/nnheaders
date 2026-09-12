#pragma once

#include <nn/types.h>

namespace nn::audio {
enum MemoryPoolState {
    MemoryPoolState_Invalid,
    MemoryPoolState_New,
    MemoryPoolState_RequestDetach,
    MemoryPoolState_Detached,
    MemoryPoolState_RequestAttach,
    MemoryPoolState_Attached,
    MemoryPoolState_Released,
};

struct MemoryPoolInfo {};

struct MemoryPoolType {
    enum State {
        State_RequestAttach,
        State_Attached,
        State_RequestDetach,
        State_Detached,
    };

    static const size_t AddressAlignment {4096};
    static const size_t SizeGranularity {4096};

    MemoryPoolInfo* _pMemoryPoolInfo;
};
static_assert(sizeof(MemoryPoolType) == 0x8);
} // namespace nn::audio
