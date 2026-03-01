#pragma once

namespace nn::atk::detail::fnd {
class RuntimeTypeInfo {
public:
private:
    RuntimeTypeInfo* m_ParentTypeInfo;
};
static_assert(sizeof(RuntimeTypeInfo) == 0x8);
} // namespace nn::atk::detail::fnd