#include <nn/gfx/detail/gfx_Device-api.nvn.8.h>
#include <nn/gfx/detail/gfx_ResShaderImpl.h>
#include <nn/gfx/gfx_ResShader.h>
#include <nn/gfx/gfx_ResShaderData-api.nvn.h>

#include "gfx_NvnHelper.h"

// todo: all these functions

namespace nn::gfx {
namespace detail {

template <>
size_t
ResShaderProgramImpl::NvnGetRecommendedScrachMemorySize<NvnApi>(const ResShaderProgram* pThis,
                                                                DeviceImpl<NvnApi>* pDevice) {
    static detail::Ptr<const void> ShaderInfoData::*s_pStageCodes[] = {
        &ShaderInfoData::pVertexShaderCode, &ShaderInfoData::pHullShaderCode,
        &ShaderInfoData::pDomainShaderCode, &ShaderInfoData::pGeometryShaderCode,
        &ShaderInfoData::pPixelShaderCode,  &ShaderInfoData::pComputeShaderCode,
    };

    const ResShaderProgram::value_type& data = pThis->ToData();
    uint32_t maxSizeRecommended = 0;
    uint32_t maxSizePerWarp = 0;

    for (int idxStage = 0; idxStage < ShaderStage_End; ++idxStage) {
        const NvnShaderCode* pNvnShaderData = data.info.*s_pStageCodes[idxStage];
        if (pNvnShaderData) {
            maxSizeRecommended =
                std::max(maxSizeRecommended, pNvnShaderData->scratchMemoryRecommended);
            maxSizePerWarp = std::max(maxSizePerWarp, pNvnShaderData->scratchMemoryPerWarp);
        }
    }

    uint32_t maxSize = 0;

    if (maxSizeRecommended != 0) {
        maxSize = maxSizeRecommended;
    } else if (maxSizePerWarp != 0) {
        int scaleFactor;
        nvnDeviceGetInteger(pDevice->ToData()->pNvnDevice,
                            NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_SCALE_FACTOR_RECOMMENDED,
                            &scaleFactor);
        maxSize = scaleFactor * maxSizePerWarp;
    } else {
        return maxSize;
    }

    int granularity;
    nvnDeviceGetInteger(pDevice->ToData()->pNvnDevice,
                        NVN_DEVICE_INFO_SHADER_SCRATCH_MEMORY_GRANULARITY, &granularity);
    return (maxSize + granularity - 1) / granularity * granularity;
}

template <>
void ResShaderContainerImpl::Initialize<NvnApi>(ResShaderContainer*, DeviceImpl<NvnApi>*,
                                                MemoryPoolImpl<NvnApi>*, ptrdiff_t, size_t) {}

template <>
void ResShaderContainerImpl::Finalize<NvnApi>(ResShaderContainer*, DeviceImpl<NvnApi>*) {}

}  // namespace detail

template <>
size_t
NvnGetMaxRecommendedScratchMemorySize<NvnApi>(TDevice<NvnApi>* pDevice,
                                              const ResShaderFile* const* ppResShaderFileArray,
                                              int shaderFileCount) {
    return 0;
}
}  // namespace nn::gfx