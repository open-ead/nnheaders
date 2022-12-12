#pragma once

#include <nn/gfx/detail/gfx_State-api.nvn.8.h>
#include <nn/nn_BitTypes.h>
#include <nn/util/util_BitPack.h>

namespace nn::gfx {

template <>
struct PipelineImplData<NvnApi> {
    enum PipelineType { PipelineType_Graphics, PipelineType_Compute };
    enum State { State_NotInitialized, State_Initialized };
    enum Flag { Flag_HasTessellationState };

    Bit8 state;
    Bit8 nnPipelineType;
    nn::util::BitPack8 flags;
    char reserved;
    Bit32 memorySize;

    detail::VertexStateImpl<NvnApi>::DataType nnVertexState;
    detail::BlendStateImpl<NvnApi>::DataType nnBlendState;
    detail::RasterizerStateImpl<NvnApi>::DataType nnRasterizerState;
    detail::DepthStencilStateImpl<NvnApi>::DataType nnDepthStencilState;
    detail::TessellationStateImpl<NvnApi>::DataType nnTessellationState;

    detail::Ptr<const detail::ShaderImpl<NvnApi>> pShader;
    detail::Ptr<void> pWorkMemory;

    detail::Ptr<void> userPtr;
};

}  // namespace nn::gfx