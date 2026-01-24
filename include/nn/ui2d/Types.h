
namespace nn::ui2d {

enum PaneFlag {
    PaneFlag_Visible,
    PaneFlag_InfluencedAlpha,
    PaneFlag_LocationAdjust,
    PaneFlag_UserAllocated,
    PaneFlag_IsGlobalMatrixDirty,
    PaneFlag_UserMatrix,
    PaneFlag_UserGlobalMatrix,
    PaneFlag_IsConstantBufferReady,
    PaneFlag_MaxPaneFlag
};

enum PaneFlagEx {
    PaneFlagEx_IgnorePartsMagnify,
    PaneFlagEx_PartsMagnifyAdjustToPartsBound,
    PaneFlagEx_ExtUserDataAnimationEnabled,
    PaneFlagEx_ViewerInvisible,
    PaneFlagEx_IsConstantBufferReadySelf,
    PaneFlagEx_IsCalculationFinishedSelf,
    PaneFlagEx_DynamicExtUserDataEnabled,
    PaneFlagEx_MaxPaneFlagEx
};

struct Size {
    static Size Create(f32, f32);

    void Set(f32 aWidth, f32 aHeight) {
        width = aWidth;
        height = aHeight;
    }

    f32 width;
    f32 height;
};
}  // namespace nn::ui2d

namespace nn::ui2d::detail {

template <typename T>
void SetBit(T* pBits, s32 pos, bool val) {
    const T mask = static_cast<T>(1 << pos);
    *pBits &= ~mask;
    *pBits |= mask * val;
}

template <typename T>
bool TestBit(T bits, s32 pos) {
    const T mask = static_cast<T>(1 << pos);
    return bits & mask;
}

}  // namespace nn::ui2d::detail
