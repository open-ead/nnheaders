#pragma once

#include <nn/types.h>

namespace nn::atk::detail {
template <typename T1, typename T2>
class MoveValue {
public:
private:
    T1 m_Origin;
    T1 m_Target;
    T2 m_Frame;
    T2 m_Counter;
};
} // namespace nn::atk::detail