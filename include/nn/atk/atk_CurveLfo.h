#pragma once

#include <nn/types.h>

namespace nn::atk::detail {
f32 CurveSine(f32 arg);
f32 CurveTriangle(f32 arg);
f32 CurveSaw(f32 arg);
f32 CurveSquare(f32 arg);
f32 CurveRandom(f32 arg);

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

    void Initialize();

    f32 depth;
    f32 speed;
    u32 delay;
    u8 range;
    u8 curve;
    u8 phase;
    u8 padding[1];
};
static_assert(sizeof(CurveLfoParam) == 0x10);

class CurveLfo {
public:
    using CurveFunc = f32(*)(f32);

    static void InitializeCurveTable();

    void RegisterUserCurve(CurveFunc curveFunc, u32);
    void UnregisterUserCurve(u32);

    void Reset();
    void Update(s32 msec);

    f32 GetValue() const;

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

static CurveLfo::CurveFunc g_CurveFuncTable[5];
} // namespace nn::atk::detail