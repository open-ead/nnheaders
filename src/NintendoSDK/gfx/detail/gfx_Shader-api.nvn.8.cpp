#include <nn/gfx/detail/gfx_Shader-api.nvn.8.h>

#include <nvnTool/nvnTool_GlslcInterface.h>

#include <cstdlib>

#include "gfx_NvnHelper.h"

namespace nn::gfx::detail {

namespace {

// todo: reverse this class, look for inline calls
class OnlineCompiledShader {
    NVNmemoryPool m_NvnMemoryPool;
    void* m_pMemoryBase;
    void* m_pMemory;
    GLSLCoutput* m_pOutput;
    NVNshaderData m_NvnShaderData[6];
    int m_StageCount;
    size_t m_ScratchMemoryPerWarp;
    const GLSLCprogramReflectionHeader* m_pReflectionHeader;

public:
    OnlineCompiledShader();

    // inlined in ShaderImpl::Finalize
    ~OnlineCompiledShader() {
        if (m_pMemoryBase) {
            nvnMemoryPoolFinalize(&m_NvnMemoryPool);
            free(m_pMemoryBase);
            m_pMemoryBase = nullptr;
            m_pMemory = nullptr;
        }

        if (m_pOutput) {
            free(m_pOutput);
            m_pOutput = nullptr;
        }

        m_pReflectionHeader = nullptr;
    }

    bool Initialize(DeviceImpl<NvnApi>*, const GLSLCoutput*);
    void Finalize();
    NVNboolean SetShader(NVNprogram*) const;
    int GetInterfaceSlot(ShaderStage, ShaderInterfaceType, const char*) const;
    size_t GetScratchMemorySizePerWarp() const;
    void GetWorkGroupSize(int*, int*, int*) const;

private:
    template <typename T>
    int SearchInterfaceSlot(const T*, int, const void*, const char*, NVNshaderStage) const;
};

}  // namespace

size_t ShaderImpl<NvnApi>::GetBinaryCodeAlignment(DeviceImpl<NvnApi>*) {
    return 256;
}

ShaderImpl<NvnApi>::ShaderImpl() {}

ShaderImpl<NvnApi>::~ShaderImpl() {}

ShaderInitializeResult ShaderImpl<NvnApi>::Initialize(DeviceImpl<NvnApi>*, const ShaderInfo&) {
    // todo: a lot of code
    return {};
}

void ShaderImpl<NvnApi>::Finalize(DeviceImpl<NvnApi>*) {
    nvnProgramFinalize(pNvnProgram);
    pNvnProgram = nullptr;

    if (pOnlineCompiledShader) {
        // inlined, is this how it should be called?
        static_cast<OnlineCompiledShader*>(pOnlineCompiledShader)->~OnlineCompiledShader();
        free(pOnlineCompiledShader);
        pOnlineCompiledShader = nullptr;
    }

    state = State_NotInitialized;
}

int ShaderImpl<NvnApi>::GetInterfaceSlot(ShaderStage, ShaderInterfaceType, const char*) const {
    // todo: a lot of code
    return {};
}

void ShaderImpl<NvnApi>::GetWorkGroupSize(int*, int*, int*) const {
    // todo: a lot of code
}

}  // namespace nn::gfx::detail