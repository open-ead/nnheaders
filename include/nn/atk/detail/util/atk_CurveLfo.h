#pragma once

#include <nn/types.h>

namespace nn::atk::detail {
struct CurveLfoParam {
    enum CurveType {
        CurveType_Min,
        CurveType_UserMin = 0x40,
        CurveType_Max = 0x7f,
        CurveType_UserMax = CurveType_Max,

        CurveType_Sine = CurveType_Min,
        CurveType_Triangle,
        CurveType_Saw,
        CurveType_Square,
        CurveType_Random,

        CurveType_Count = 0x80,
    };

    f32 depth;
    f32 speed;
    u32 delay;
    u8 range;
    u8 curve;
    u8 phase;
    u8 padding[1];

    void Initialize();
};
static_assert(sizeof(CurveLfoParam) == 0x10);

class CurveLfo {
public:
    using CurveFunc = f32(*)(f32);

    static void InitializeCurveTable();

    void Reset();
    void Update(s32 msec);

    f32 GetValue();

private:
    CurveLfoParam m_Param;
    u32 m_DelayCounter;
    f32 m_Counter;
    f32 m_RandomValue;
    bool m_IsStart;
    bool m_IsNext;
    u8 m_Padding[2];
};
static_assert(sizeof(CurveLfo) == 0x20);
} // namespace nn::atk::detail