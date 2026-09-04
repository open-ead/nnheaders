#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

struct CurveLfoParam {
    float depth;
    float speed;
    u32 delay;
    u8 range;
    u8 curve;
    u8 phase;
    u8 padding[1];

    enum CurveType {
        CurveType_Min,

        CurveType_Sine = CurveType_Min,
        CurveType_Triangle,
        CurveType_Saw,
        CurveType_Square,
        CurveType_Random,

        CurveType_UserMin = 64,
        CurveType_Max = 127,
        CurveType_UserMax = CurveType_Max,

        CurveType_Count,
    };

    CurveLfoParam();

    void Initialize();
};
static_assert(sizeof(CurveLfoParam) == 0x10);

class CurveLfo {
public:

    CurveLfo() = default;
    
    void Reset();
    void Update(int msec);

    float GetValue() const;

    void SetParam(const CurveLfoParam& param) { m_Param = param; }
    CurveLfoParam& GetParam() { return m_Param; }
    const CurveLfoParam& GetParam() const { return m_Param; }

    static void InitializeCurveTable();

    using CurveFunc = float(*)(float);

    void RegisterUserCurve(CurveFunc func, u32 index);
    void UnregisterUserCurve(u32 index);

private:
    CurveLfoParam m_Param;
    u32 m_DelayCounter;
    float m_Counter;
    float m_RandomValue;
    bool m_IsStart;
    bool m_IsNext;
    u8 m_Padding[2];
};
static_assert(sizeof(CurveLfo) == 0x20);

} // namespace nn::atk::detail