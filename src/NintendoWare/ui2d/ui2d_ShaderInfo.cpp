#include <nn/ui2d/ui2d_ShaderInfo.h>

#include <nn/gfx/gfx_ResShader.h>
#include <nn/gfx/gfx_Shader.h>
#include <nn/gfx/gfx_StateInfo.h>
#include <nn/ui2d/ui2d_Layout.h>

namespace nn {
namespace ui2d {

void ShaderInfo::Initialize(gfx::Device* pDevice, void* pShader) {
    InitializeWithVariationTable(pDevice, pShader, nullptr, nullptr, 0, 0);
}

/*
void ShaderInfo::InitializeWithVariationTable(gfx::Device* pDevice, void* pShader,
const void* pVariationTable,
gfx::MemoryPool* pMemoryPool,
ptrdiff_t memoryPoolOffset, size_t memoryPoolSize) {
// unused with being inlined into Initialize, need to investigate for smo
NN_UNUSED(pVariationTable);
NN_UNUSED(pMemoryPool);
NN_UNUSED(memoryPoolOffset);
NN_UNUSED(memoryPoolSize);

m_pResShaderFile = gfx::ResShaderFile::ResCast(pShader);

// { there should be a scope here but I can't see what is causing it
gfx::ResShaderContainer* pContainer = m_pResShaderFile->GetShaderContainer();
if (!pContainer || ) {
pContainer->Initialize(pDevice);

/ figure this out with smo
util::BytePtr resShaderFileTop;
pContainer->Initialize(pDevice, pMemoryPool, memoryPoolOffset, memoryPoolSize);
resShaderFileTop.Distance(pContainer);
/
}
{ ResShaderVariation* pVariation = pContainer->GetResShaderVariation(0); }

for (int i = 1, variationCount = GetVariationCount(); i < variationCount; ++i) {
gfx::ResShaderVariation* pVariation = pContainer->GetResShaderVariation(i);
const bool isShaderInitialize = pVariation->GetResShaderProgram(gfx::ShaderCodeType_Binary);
gfx::ShaderInitializeResult shaderResult =
pVariation->GetResShaderProgram(gfx::ShaderCodeType_Binary)->Initialize();
}
//}

{
int variationCount = GetVariationCount();
Layout::NewArray<gfx::VertexState>(variationCount);
}

for (int i = 0, variationCount = GetVariationCount(); i < variationCount; ++i) {
gfx::Shader* pVertexShader = GetVertexShader(i);
gfx::Shader* pGeometryShader = GetVertexShader(i);
gfx::Shader* pPixelShader = GetVertexShader(i);
pVertexShader->GetInterfaceSlot(gfx::ShaderStage_Vertex,
gfx::ShaderInterfaceType_ConstantBuffer, "");
pGeometryShader->GetInterfaceSlot(gfx::ShaderStage_Geometry,
gfx::ShaderInterfaceType_ConstantBuffer, "");
pPixelShader->GetInterfaceSlot(gfx::ShaderStage_Pixel,
gfx::ShaderInterfaceType_ConstantBuffer, "");
pPixelShader->GetInterfaceSlot(gfx::ShaderStage_Pixel, gfx::ShaderInterfaceType_Sampler,
"");
pPixelShader->GetInterfaceSlot(gfx::ShaderStage_Pixel, gfx::ShaderInterfaceType_Sampler,
"");
pPixelShader->GetInterfaceSlot(gfx::ShaderStage_Pixel, gfx::ShaderInterfaceType_Sampler,
"");
}
}
*/

void ShaderInfo::InitializeVertexStates(gfx::Device* pDevice) {
    for (int i = 0, variationCount = GetVariationCount(); i < variationCount; ++i) {
        gfx::Shader* pVertexShader = GetVertexShader(i);
        gfx::VertexStateInfo info;
        info.SetDefault();

        gfx::VertexAttributeStateInfo attribs[3];
        int attribCount = 0;
        int stride = 0;

        if (pVertexShader->GetInterfaceSlot(gfx::ShaderStage_Vertex, gfx::ShaderInterfaceType_Input,
                                            "aVertexIndex") >= 0) {
            attribs[attribCount].SetDefault();
            attribs[attribCount].SetNamePtr("aVertexIndex");
            attribs[attribCount].SetBufferIndex(0);
            attribs[attribCount].SetOffset(stride);
            attribs[attribCount].SetFormat(gfx::AttributeFormat_32_32_Float);

            ++attribCount;
            stride += 8;
        }

        if (pVertexShader->GetInterfaceSlot(gfx::ShaderStage_Vertex, gfx::ShaderInterfaceType_Input,
                                            "aPosition") >= 0) {
            attribs[attribCount].SetDefault();
            attribs[attribCount].SetNamePtr("aPosition");
            attribs[attribCount].SetBufferIndex(0);
            attribs[attribCount].SetOffset(stride);
            attribs[attribCount].SetFormat(gfx::AttributeFormat_32_32_32_Float);

            ++attribCount;
            stride += 12;
        }

        if (pVertexShader->GetInterfaceSlot(gfx::ShaderStage_Vertex, gfx::ShaderInterfaceType_Input,
                                            "aTexCoord") >= 0) {
            attribs[attribCount].SetDefault();
            attribs[attribCount].SetNamePtr("aTexCoord");
            attribs[attribCount].SetBufferIndex(0);
            attribs[attribCount].SetOffset(stride);
            attribs[attribCount].SetFormat(gfx::AttributeFormat_32_32_Float);

            ++attribCount;
            stride += 8;
        }

        gfx::VertexBufferStateInfo buffer[1];
        buffer[0].SetDefault();
        buffer[0].SetStride(stride);
        buffer[0].SetDivisor(0);

        info.SetVertexAttributeStateInfoArray(attribs, attribCount);
        info.SetVertexBufferStateInfoArray(buffer, 1);

        size_t size = gfx::VertexState::GetRequiredMemorySize(info);
        m_pVertexStates[i].SetMemory(Layout::AllocateMemory(size, 8), size);
        m_pVertexStates[i].Initialize(pDevice, info, pVertexShader);
    }
}
/*
void ShaderInfo::Finalize(gfx::Device*, bool);
void ShaderInfo::SetShader(gfx::CommandBuffer&, int) const;
*/

gfx::Shader* ShaderInfo::GetVertexShader(int variation) const {
    return m_pResShaderFile->GetShaderContainer()
        ->GetResShaderVariation(variation)
        ->GetResShaderProgram(m_CodeType)
        ->GetShader();
}

gfx::Shader* ShaderInfo::GetGeometryShader(int variation) const {
    return m_pResShaderFile->GetShaderContainer()
        ->GetResShaderVariation(variation)
        ->GetResShaderProgram(m_CodeType)
        ->GetShader();
}

gfx::Shader* ShaderInfo::GetPixelShader(int variation) const {
    return m_pResShaderFile->GetShaderContainer()
        ->GetResShaderVariation(variation)
        ->GetResShaderProgram(m_CodeType)
        ->GetShader();
}

int ShaderInfo::GetVariationCount() const {
    return m_pResShaderFile->GetShaderContainer()->GetShaderVariationCount();
}

}  // namespace ui2d
}  // namespace nn
