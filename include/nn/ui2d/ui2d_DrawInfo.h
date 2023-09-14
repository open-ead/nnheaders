#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_StateInfo.h>
#include <nn/util/util_MathTypes.h>

namespace nn {

namespace font {

class GpuBuffer;

}

namespace ui2d {

class DrawInfo;
class GraphicsResource;
class Pane;

typedef void (*UpdateUserShaderCallback)(const DrawInfo&, Pane*, void*);
/* newer
typedef bool (*ApplyCaptureTextureFilterCallback)(gfx::CommandBuffer&, const gfx::ColorTargetView*,
                                                  const gfx::DescriptorSlot&,
                                                  const gfx::DescriptorSlot&,
                                                  const gfx::ViewportScissorState&, Pane*, void*);
*/

class DrawInfo {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    DrawInfo();
    DrawInfo(const DrawInfo&);
    virtual ~DrawInfo();
    DrawInfo& operator=(const DrawInfo&);
    void Set(const DrawInfo&);
    void ConfigureBeforeDrawing(Layout*);
    void ConfigureAfterDrawing();
    const util::MatrixT4x3fType& GetViewMtx() const;
    void SetViewMtx(const util::MatrixT4x3fType&);
    bool IsLocationAdjustEnabled() const;
    void SetLocationAdjustEnabled(bool);
    const util::Float2& GetLocationAdjustScale() const;
    void SetLocationAdjustScale(const util::Float2&);
    bool IsInvisiblePaneCalculated() const;
    void SetInvisiblePaneCalculated(bool);
    bool IsAlphaZeroPaneCalculated() const;
    void SetAlphaZeroPaneCalculated(bool);
    bool IsLeftTopWindowOrigin() const;
    void SetLeftTopWindowOrigin(bool);
    GraphicsResource* GetGraphicsResource() const;
    void SetGraphicsResource(GraphicsResource*);
    void SetProjectionMtx(const util::MatrixT4x4fType&);
    void SetUi2dConstantBuffer(font::GpuBuffer*);
    font::GpuBuffer* GetUi2dConstantBuffer() const;
    void SetFontConstantBuffer(font::GpuBuffer*);
    font::GpuBuffer* GetFontConstantBuffer() const;
    void SetUpdateUserShaderCallback(UpdateUserShaderCallback, void*);
    UpdateUserShaderCallback GetUpdateUserShaderCallback() const;
    void* GetUpdateUserShaderCallbackUserData() const;

    /*
    void SetApplyCaptureTextureFilterCallback(ApplyCaptureTextureFilterCallback, void*);
    ApplyCaptureTextureFilterCallback GetApplyCaptureTextureFilterCallback() const;
    void* GetApplyCaptureTextureFilterCallbackUserData() const;
    */

    void ResetDrawState();
    void Map(int);
    void Unmap();
    void SetGpuAccessBufferIndex(int);

    /* newer
    void SetFramebufferTexture(gfx::Texture*, int, int);
    const gfx::Texture* GetFramebufferTexture() const;
    int GetFramebufferWidth() const;
    int GetFramebufferHeight() const;
    void SetDefaultViewportScissorInfo(const gfx::ViewportStateInfo&, const gfx::ScissorStateInfo&);
    const gfx::ViewportStateInfo& GetDefaultViewportInfo() const;
    const gfx::ScissorStateInfo& GetDeafultScissorInfo() const;
    const gfx::ScissorStateInfo& GetDefaultScissorInfo() const;
    bool IsValidDefaultViewportInfoSet() const;
    bool IsValidDefaultScissorInfoSet() const;
    */

    Layout* GetLayout() { return m_pLayout; }
    const Layout* GetLayout() const { return m_pLayout; }
    void SetLayout(Layout* pLayout) { m_pLayout = pLayout; }

    const util::MatrixT4x4fType& GetProjectionMtx() const;
    void LoadProjectionMtx(float (*)[4]);
    const util::MatrixT4x3fType& GetModelViewMtx() const;
    void SetModelViewMtx(const util::MatrixT4x3fType&);
    int GetTexCoordSrc(int) const;
    void SetTexCoordSrc(int, int);
    void LoadMtxModelView(float (*)[4]);
    void SetupProgram(gfx::CommandBuffer*);

    void ResetVertexBufferState() { m_FirstDraw = true; }

    bool RecordCurrentShader(const ShaderInfo*, uint8_t);
    // smo
    // void ResetCurrentShader();
    void SetAllowableTextureQuantity(int);
    int GetAllowableTextureQuantity() const;
    void ResetModelViewMtxLoaded();

private:
    util::MatrixT4x4fType m_ProjMtx;
    util::MatrixT4x3fType m_ViewMtx;
    util::MatrixT4x3fType m_ModelViewMtx;
    util::Float2 m_LocationAdjustScale;
    GraphicsResource* m_pGraphicsResource;
    Layout* m_pLayout;
    font::GpuBuffer* m_pUi2dConstantBuffer;
    font::GpuBuffer* m_pFontConstantBuffer;

    /* newer
    gfx::Texture* m_pFramebufferTexture;
    uint32_t m_FramebufferWidth;
    uint32_t m_FramebufferHeight;
    gfx::ViewportStateInfo m_DefaultViewportInfo;
    gfx::ScissorStateInfo m_DefaultScissorInfo;
    */

    UpdateUserShaderCallback m_pUpdateUserShaderCallback;
    void* m_pUpdateUserShaderCallbackUserData;

    /* newer
    ApplyCaptureTextureFilterCallback m_pApplyCaptureTextureFilterCallback;
    void* m_pApplyCaptureTextureFilterCallbackUserData;
    */

    int8_t m_TexCoordSrc[3];

public:
    char filler[3];

private:
    bool m_ModelViewMtxLoaded;
    bool m_FirstDraw;
    int8_t m_AllowableTextureQuantity;

    struct Flag {
        uint8_t isLocationAdjustEnabled : 1;
        uint8_t invisiblePaneCalculateMtx : 1;
        uint8_t alphaZeroPaneCalculateMtx : 1;
        uint8_t isLeftTopWindowOrigin : 1;
    };

    Flag m_Flag;
    const ShaderInfo* m_pCurrentShaderInfo;
    uint8_t m_CurrentShaderVariation;
};

}  // namespace ui2d
}  // namespace nn
