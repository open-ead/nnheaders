#pragma once

#include <nn/gfx/detail/gfx_RootSignature-api.common.h>
#include <nn/gfx/gfx_Common.h>

namespace nn::gfx::detail {
template <>
class RootSignatureImpl<NvnApi> : public RootSignatureImplCommon<NvnApi> {};
}  // namespace nn::gfx::detail