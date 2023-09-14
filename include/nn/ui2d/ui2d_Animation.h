#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Types.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {

namespace font {
namespace detail {

struct BinaryFileHeader;

}
}  // namespace font

namespace ui2d {

struct ResAnimationContent;
struct ResAnimationBlock;
struct ResAnimationTagBlock;
struct ResAnimationShareBlock;
class TextureInfo;
class Pane;
class Layout;
class Material;
class ResourceAccessor;
struct ResAnimationBlock;
class Group;
class ResExtUserData;
struct ResAnimationGroupRef;
struct ResExtUserDataList;
struct ResAnimationShareInfo;

class AnimTransform {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    class BindPair {
    public:
        void* target;
        const ResAnimationContent* animCont;

        BindPair() : target(nullptr), animCont(nullptr) {}
    };

    AnimTransform();
    virtual ~AnimTransform();

    float GetFrame() const { return m_Frame; }
    void SetFrame(float);

    uint16_t GetFrameSize() const;

    float GetFrameMax() const;

    virtual void UpdateFrame(float progressFrame);

    const ResAnimationBlock* GetAnimResource() const { return m_pRes; }
    bool IsEnabled() const { return m_IsEnabled; }

    virtual void SetEnabled(bool bEnable);
    bool IsLoopData() const;
    bool IsWaitData() const;

    virtual void Animate() = 0;
    virtual void AnimatePane(Pane*) = 0;
    virtual void AnimateMaterial(Material*) = 0;
    virtual void SetResource(gfx::Device*, ResourceAccessor*, const ResAnimationBlock*) = 0;
    virtual void SetResource(gfx::Device*, ResourceAccessor*, const ResAnimationBlock*,
                             uint16_t) = 0;
    virtual void BindPane(Pane*, bool) = 0;
    virtual void BindGroup(Group*) = 0;
    virtual void BindMaterial(Material*) = 0;
    virtual void ForceBindPane(Pane*, const Pane*) = 0;
    virtual void UnbindPane(const Pane*) = 0;
    virtual void UnbindGroup(const Group*) = 0;
    virtual void UnbindMaterial(const Material*) = 0;
    virtual void UnbindAll() = 0;

    util::IntrusiveListNode m_Link;

protected:
    void SetAnimResource(const ResAnimationBlock* pRes) { m_pRes = pRes; }

private:
    const ResAnimationBlock* m_pRes;
    float m_Frame;
    bool m_IsEnabled;
};

class AnimTransformBasic : public AnimTransform {
public:
    NN_RUNTIME_TYPEINFO(AnimTransform);

    AnimTransformBasic();
    virtual ~AnimTransformBasic();
    virtual void Animate();
    virtual void AnimatePane(Pane*);
    virtual void AnimateMaterial(Material*);
    virtual void SetResource(gfx::Device*, ResourceAccessor*, const ResAnimationBlock*);
    virtual void SetResource(gfx::Device*, ResourceAccessor*, const ResAnimationBlock*, uint16_t);
    virtual void BindPane(Pane*, bool);
    virtual void BindGroup(Group*);
    virtual void BindMaterial(Material*);
    virtual void ForceBindPane(Pane*, const Pane*);
    virtual void UnbindPane(const Pane*);
    virtual void UnbindGroup(const Group*);
    virtual void UnbindMaterial(const Material*);
    virtual void UnbindAll();
    bool BindPaneImpl(Pane*, const ResAnimationContent*);
    bool BindMaterialImpl(Material*, const ResAnimationContent*);
    bool BindExtUserDataImpl(ResExtUserData*, const ResAnimationContent*);

protected:
    virtual void AnimatePaneImpl(Pane*, const ResAnimationContent*);
    virtual void AnimateMaterialImpl(Material*, const ResAnimationContent*);
    virtual void AnimateExtUserDataImpl(ResExtUserData*, const ResAnimationContent*);
    bool BindExtUserDataToPane(Pane*, const ResAnimationContent&);
    bool CheckBindAnimationDoubly(const void*, const ResAnimationContent*) const;
    void EraseBindPair(int);

    const TextureInfo** m_pTextures;
    AnimTransform::BindPair* m_pBindPairs;
    uint16_t m_BindPairCount;
    uint16_t m_BindPairCountMax;
};

class AnimResource {
public:
    AnimResource();

    explicit AnimResource(const void* pAnimResBuf) { Set(pAnimResBuf); }

    void Set(const void*);
    const font::detail::BinaryFileHeader* GetFileHeader() const;
    const ResAnimationBlock* GetResourceBlock() const { return m_pResBlock; }
    const ResAnimationTagBlock* GetTagBlock() const;
    uint16_t GetTagOrder() const;
    const char* GetTagName() const;
    uint16_t GetGroupCount() const;
    const ResAnimationGroupRef* GetGroupArray() const;
    const ResExtUserDataList* GetExtUserDataList() const;
    bool IsDescendingBind() const;
    uint16_t GetAnimationShareInfoCount() const;
    const ResAnimationShareInfo* GetAnimationShareInfoArray() const;
    uint16_t CalculateAnimationCount(Pane*, bool) const;
    uint16_t CalculateAnimationCount(Material*) const;
    uint16_t CalculateAnimationCount(Group*, bool) const;

private:
    void Initialize();
    bool CheckResource() const;

    const font::detail::BinaryFileHeader* m_pFileHeader;
    const ResAnimationBlock* m_pResBlock;
    const ResAnimationTagBlock* m_pTagBlock;
    const ResAnimationShareBlock* m_pShareBlock;
};

namespace detail {

class AnimPaneTree {
public:
    AnimPaneTree();
    AnimPaneTree(Pane*, const AnimResource&);
    void Set(Pane*, const AnimResource&);
    AnimTransform* Bind(gfx::Device*, Layout*, Pane*, ResourceAccessor*) const;

    bool IsEnabled() const { return m_LinkCount != 0; }
    const AnimResource& GetAnimResource() const { return m_AnimRes; }

private:
    static const ResAnimationContent* FindAnimContent(const ResAnimationBlock*, const char*,
                                                      uint8_t);
    void Initialize();

    static const int MaterialCountMax = 9;
    AnimResource m_AnimRes;
    const ResAnimationContent* m_pPaneAnimContent;
    const ResAnimationContent* m_pMatAnimContents[MaterialCountMax];
    uint16_t m_LinkCount;
    uint8_t m_AnimMatCnt;
};

}  // namespace detail
}  // namespace ui2d
}  // namespace nn
