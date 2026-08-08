#pragma once

#include <nn/types.h>

namespace nn::atk::detail {
template <typename T>
class ValueArray {
public:
private:
    T* m_pValue;
    s32 m_Count;
};
} // namespace nn::atk::detail