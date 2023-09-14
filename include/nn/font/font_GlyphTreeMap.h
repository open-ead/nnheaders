#pragma once

#include <nn/font/detail/font_FreeList.h>
#include <nn/nn_Allocator.h>
#include <nn/util/util_BitPack.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn {
namespace font {

class GlyphNode {
public:
    union KeyType {
        uint64_t raw;
        struct {
            uint32_t code;
            uint16_t fontFace;
            uint16_t fontSize;
        } detail;
    };

    enum FlagMask {
        FlagMask_Requested = 1,
        FlagMask_Plotting,
        FlagMask_Used = 4,
        FlagMask_Erased = 8,
        FlagMask_Kept = 16,
        FlagMask_SystemReserved = 32,
        FlagMask_NotInFont = 64
    };

    util::IntrusiveListNode m_Link;
    util::IntrusiveListNode m_LineLink;

private:
    static const uint8_t Red;
    static const uint8_t Black;
    GlyphNode* m_pLeftNode;
    GlyphNode* m_pRightNode;
    KeyType m_Key;
    util::BitPack8 m_Flag;
    uint8_t m_Color;
    uint8_t m_LineKind;
    uint8_t m_LineNo;
    util::BitPack32 m_LockGroup;
    uint16_t m_CachePosX;
    uint16_t m_CachePosY;
    uint16_t m_CacheWidth;
    uint16_t m_CacheHeight;
    uint16_t m_GlyphWidth;
    uint16_t m_GlyphHeight;
    uint16_t m_AdvanceX;
    int16_t m_LeftOffset;
    int16_t m_BaselineOffset;

public:
    GlyphNode(uint32_t, uint16_t, uint16_t);
    uint32_t GetCode() const;
    uint16_t GetSize() const;
    uint16_t GetFace() const;
    void SetRequested(bool);
    bool IsRequested() const;
    void SetPlotting(bool);
    bool IsPlotting() const;
    void SetUsed(bool);
    bool IsUsed() const;
    void SetErased(bool);
    bool IsErased() const;
    void SetKeeped(bool);
    bool IsKeeped() const;
    void SetSystemReserved(bool);
    bool IsSystemReserved() const;
    void SetNotInFont(bool);
    bool IsNotInFont() const;
    bool IsPlottingOrNotInFont() const;
    bool IsRequestedOrKeeped() const;
    uint16_t GetCachePosX() const;
    uint16_t GetCachePosY() const;
    uint16_t GetCacheWidth() const;
    uint16_t GetCacheHeight() const;
    uint16_t GetGlyphWidth() const;
    uint16_t GetGlyphHeight() const;
    uint16_t GetAdvanceX() const;
    int16_t GetLeftOffset() const;
    int16_t GetBaselineOffset() const;
    void SetCachePosX(uint16_t);
    void SetCachePosY(uint16_t);
    void SetCacheWidth(uint16_t);
    void SetCacheHeight(uint16_t);
    void SetGlyphWidth(uint16_t);
    void SetGlyphHeight(uint16_t);
    void SetAdvanceX(uint16_t);
    void SetLeftOffset(int16_t);
    void SetBaselineOffset(int16_t);
    util::BitPack32& GetLockGroup();
    const util::BitPack32& GetLockGroup() const;
    uint8_t GetLineNo() const;
    void SetLineNo(uint8_t);
    uint8_t GetLineKind() const;
    void SetLineKind(uint8_t);
    bool IsErasable() const;
    bool IsUsable() const;
    static uint8_t CalculateLineKind(uint16_t);
    static uint32_t CalculateLineHeight(uint8_t);
};

typedef util::IntrusiveList<
    GlyphNode, util::IntrusiveListMemberNodeTraits<GlyphNode, &GlyphNode::m_Link, GlyphNode>>
    GlyphList;
typedef util::IntrusiveList<
    GlyphNode, util::IntrusiveListMemberNodeTraits<GlyphNode, &GlyphNode::m_LineLink, GlyphNode>>
    GlyphLineList;

// todo: reorder
class GlyphTreeMap {
    GlyphNode* m_pRootNode;
    detail::FreeList m_FreeList;
    uint32_t m_NodeCountMax;

public:
    GlyphTreeMap();
    void Initialize(AlignedAllocateFunctionWithUserData, void*, uint32_t);
    void Finalize(FreeFunctionWithUserData, void*);
    GlyphNode* Find(uint32_t, uint16_t, uint16_t) const;

private:
    GlyphNode* Find(GlyphNode*, uint64_t) const;

public:
    GlyphNode* Insert(uint32_t, uint16_t, uint16_t);

private:
    GlyphNode* Insert(GlyphNode*, GlyphNode*);

public:
    void Erase(uint32_t, uint16_t, uint16_t);

private:
    GlyphNode* Erase(GlyphNode*, uint64_t);

public:
    void UpdateFlagsForCompleteTextureCache();
    void ClearLockGroup(uint32_t);
    void Dump() const;
    void Reset();

private:
    static GlyphNode* RotateLeft(GlyphNode*);
    static GlyphNode* RotateRight(GlyphNode*);
    static void FlipColors(GlyphNode*);
    static bool IsRed(GlyphNode*);
    static GlyphNode* FixUp(GlyphNode*);
    static GlyphNode* MoveRedLeft(GlyphNode*);
    static GlyphNode* MoveRedRight(GlyphNode*);
    static GlyphNode* EraseMin(GlyphNode*);
    static GlyphNode* GetMin(GlyphNode*);
    static void UpdateFlagsForCompleteTextureCacheRecursive(GlyphNode*);
    static void ClearLockGroupRecursive(GlyphNode*, uint32_t);
    static uint32_t DumpRecursive(GlyphNode*, uint32_t);
};
}  // namespace font
}  // namespace nn
