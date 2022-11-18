#pragma once

#include <nn/gfx/detail/gfx_DataContainer.h>
#include <nn/gfx/gfx_Common.h>
#include <nn/gfx/gfx_Enum.h>
#include <nn/gfx/gfx_ShaderData-api.nvn.8.h>

namespace nn::gfx {
class ShaderInfo;

namespace detail {

template <>
class ShaderImpl<NvnApi> : public DataContainer<ShaderImplData<NvnApi>> {
    NN_NO_COPY(ShaderImpl);

public:
    typedef ShaderInfo InfoType;

    ShaderImpl();

    static size_t GetBinaryCodeAlignment(DeviceImpl<NvnApi>*);

    ~ShaderImpl();
    ShaderInitializeResult Initialize(DeviceImpl<NvnApi>*, const InfoType&);
    void Finalize(DeviceImpl<NvnApi>*);
    int GetInterfaceSlot(ShaderStage, ShaderInterfaceType, const char*) const;
    void GetWorkGroupSize(int*, int*, int*) const;
};

}  // namespace detail
}  // namespace nn::gfx