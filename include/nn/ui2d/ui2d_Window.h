#pragma once

#include <nn/ui2d/ui2d_Pane.h>

#include <nn/ui2d/ui2d_WindowFoundation.h>

namespace nn {
namespace ui2d {

class Material;

class Window : public Pane {
    class Frame {
    public:
        uint8_t textureFlip;
        Material* pMaterial;

        Frame();
        ~Frame();
        TextureFlip GetTextureFlip() const;
    };

public:
    NN_RUNTIME_TYPEINFO(Pane);

    Window(int, int);
    Window(int, int, int, int, int);
    Window(int, int, int, int, int, int, int, int, int);
    Window(BuildResultInformation*, gfx::Device*, const ResWindow*, const ResWindow*,
           const BuildArgSet&);
    Window(const Window&, gfx::Device*);
    Window(const Window&, gfx::Device*, ResourceAccessor*, const char*);

    virtual ~Window();
    virtual void Finalize(gfx::Device*);
    virtual const util::Unorm8x4 GetVertexColor(int) const;
    virtual void SetVertexColor(int, const util::Unorm8x4&);
    virtual uint8_t GetVertexColorElement(int) const;
    virtual void SetVertexColorElement(int, uint8_t);
    virtual uint8_t GetMaterialCount() const;
    virtual Material* GetMaterial(int) const;
    Material* GetContentMaterial() const;
    void SetContentMaterial(Material*);
    Material* GetFrameMaterial(WindowFrame) const;
    void SetFrameMaterial(WindowFrame, Material*);
    void ReserveTexCoord(int);
    uint8_t GetTexCoordCount() const;
    void SetTexCoordCount(int);
    void GetTexCoord(util::Float2*, int) const;
    void SetTexCoord(int, const util::Float2*);
    uint8_t GetFrameCount() const;
    void CalculateFrameSize(WindowFrameSize*);

    void SetWindowFrameSize(int index, int newSize) {
        switch (index) {
        case AnimTargetWindow_FrameTop:
            m_WindowSize.frameSize.top = newSize;
            break;
        case AnimTargetWindow_FrameBottom:
            m_WindowSize.frameSize.bottom = newSize;
            break;
        case AnimTargetWindow_FrameLeft:
            m_WindowSize.frameSize.left = newSize;
            break;
        case AnimTargetWindow_FrameRight:
            m_WindowSize.frameSize.right = newSize;
            break;
        default:
            break;
        }
    }

    WindowFrameMode GetWindowFrameMode() const;
    virtual Material* FindMaterialByName(const char*, bool);
    virtual const Material* FindMaterialByName(const char*, bool) const;
    virtual void Calculate(DrawInfo&, Pane::CalculateContext&, bool);
    bool CompareCopiedInstanceTest(const Window&) const;

protected:
    virtual void DrawSelf(DrawInfo&, gfx::CommandBuffer&);
    void CopyImpl(const Window&, gfx::Device*, ResourceAccessor*, const char*);

private:
    void InitializeTexCount(int, int*, int);
    void InitializeUseLeftTopMaterialEmulation(BuildResultInformation*, gfx::Device*);
    void InitializeSize(const ResWindow*);
    void InitializeContent(int);
    void InitializeFrame(int);
    void CalculateAroundFrameWindow(DrawInfo&);
    void CalculateHorizontalFrameWindow(DrawInfo&);
    void CalculateHorizontalFrameNocontentWindow(DrawInfo&);
    void CalculateContent(DrawInfo&, const util::Float2&, const WindowFrameSize&, uint8_t);
    void CalculateFrame(DrawInfo&, const util::Float2&, const Frame&, const WindowFrameSize&,
                        uint8_t);
    void CalculateHorizontalFrame(DrawInfo&, const util::Float2&, const Frame&,
                                  const WindowFrameSize&, uint8_t);
    void CalculateHorizontalNocontentFrame(DrawInfo&, const util::Float2&, const Frame&,
                                           const WindowFrameSize&, uint8_t);
    void CalculateHorizontalFrame2(DrawInfo&, const util::Float2&, const Frame*,
                                   const WindowFrameSize&, uint8_t);
    void CalculateHorizontalNocontentFrame2(DrawInfo&, const util::Float2&, const Frame*,
                                            const WindowFrameSize&, uint8_t);
    void CalculateFrame4(DrawInfo&, const util::Float2&, const Frame*, const WindowFrameSize&,
                         uint8_t);
    void CalculateFrame8(DrawInfo&, const util::Float2&, const Frame*, const WindowFrameSize&,
                         uint8_t);

    const Window& operator=(const Window&) = delete;

    void Initialize();

    enum WindowFlags {
        WindowFlags_OverallVertexColorEnabled = 1,
        WindowFlags_OneMaterialForAllFrameEnabled = 2,
        WindowFlags_NotDrawContentEnabled = 4,
        WindowFlags_ContentInflationEnabled = 8
    };

    enum CaptureTextureCopyTarget {
        CaptureTextureCopyTarget_Content,
        CaptureTextureCopyTarget_Frame0,
        CaptureTextureCopyTarget_Frame1,
        CaptureTextureCopyTarget_Frame2,
        CaptureTextureCopyTarget_Frame3,
        CaptureTextureCopyTarget_Frame4,
        CaptureTextureCopyTarget_Frame5,
        CaptureTextureCopyTarget_Frame6,
        CaptureTextureCopyTarget_Frame7,
        CaptureTextureCopyTarget_Max
    };

    WindowSize m_WindowSize;
    WindowContent m_Content;
    uint8_t m_FrameMode;
    int8_t m_FrameCount;
    uint8_t m_WindowFlags;
    Frame* m_pFrames;
    Material* m_pMaterial;
    uint32_t* m_pUseLeftTopEmulationConstantBuffersOffset;
    uint32_t m_UseLeftTopEmulationConstantBufferCount;
};
}  // namespace ui2d
}  // namespace nn
