#pragma once

#include <nn/gfx/util/gfx_PrimitiveShape.h>
#include <nn/ui2d/ui2d_Common.h>
#include <nn/ui2d/ui2d_Pane.h>

namespace nn {
namespace ui2d {

class Material;

class Picture : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane);

    Picture(int);
    Picture(const TextureInfo&);
    Picture(BuildResultInformation*, gfx::Device*, const ResPicture*, const ResPicture*,
            const BuildArgSet&);
    Picture(const Picture&, gfx::Device*);
    Picture(const Picture&, gfx::Device*, ResourceAccessor*, const char*);

    virtual ~Picture();
    virtual void Finalize(gfx::Device*);
    virtual void Append(const TextureInfo&);
    virtual const util::Unorm8x4 GetVertexColor(int) const;
    virtual void SetVertexColor(int, const util::Unorm8x4&);
    virtual uint8_t GetVertexColorElement(int) const;
    virtual void SetVertexColorElement(int, uint8_t);
    void ReserveTexCoord(int);
    int GetTexCoordCount() const;
    void SetTexCoordCount(int);
    void GetTexCoord(util::Float2*, int) const;
    void SetTexCoord(int, const util::Float2*);
    virtual uint8_t GetMaterialCount() const;
    virtual Material* GetMaterial(int) const;
    void SetMaterial(Material*);
    virtual void Calculate(DrawInfo&, Pane::CalculateContext&, bool);
    bool CompareCopiedInstanceTest(const Picture&) const;

protected:
    virtual void DrawSelf(DrawInfo&, gfx::CommandBuffer&);
    virtual void LoadMtx(DrawInfo&);
    void CopyImpl(const Picture&, gfx::Device*, ResourceAccessor*, const char*);

    struct ShapeDrawInfo {
        uint16_t m_ShapeType;
        uint8_t padding[2];
        size_t vertexBufferGpuMemoryOffset;
        size_t indexBufferGpuMemoryOffset;
        gfx::util::PrimitiveShape* pShape;
    };

    struct ShapeDrawInfoAccessor {
        uint8_t unionTypeFlag;
        uint8_t padding;
        ShapeDrawInfo* pShapeDrawInfo;
    };

private:
    static const gfx::util::PrimitiveShapeFormat VertexFormat_PUv;

    void Initialize(int);
    void InitializeMaterial(int);
    void InitializeShape(BuildResultInformation*, gfx::Device*, const ResPicture*,
                         const BuildArgSet&, uint32_t);
    void CopyShapeInfo(const Picture&);
    void CopyShapeVertexData(DrawInfo&);
    bool CheckAdditionalInfoFlag(const ResPicture*, PicturePaneFlag) const;
    bool IsTexCoordArrayEnabled() const;

    Material* m_pMaterial;
    util::Unorm8x4 m_VertexColors[4];

    union SharedMemory {
        enum { UnionType_ShapeDrawInfo = 4, UnionType_Max };

        uint8_t m_UnionTypeFlag;
        detail::TexCoordArray m_TexCoordArray;
        ShapeDrawInfoAccessor m_ShapeDrawInfoAccessor;

    public:
        SharedMemory();
        ~SharedMemory();
    };

    SharedMemory m_SharedMemory;

    const Picture& operator=(const Picture&) = delete;
};
}  // namespace ui2d
}  // namespace nn
