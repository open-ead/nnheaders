#pragma once

#include <nn/util/MathTypes.h>
#include <nn/util/detail/util_ArithmeticImpl.h>

namespace nn::util {

inline AngleIndex RadianToAngleIndex(float radian) {
    return static_cast<int64_t>(radian * (detail::AngleIndexHalfRound / detail::FloatPi));
}

inline float DegreeToRadian(float degree) {
    return degree * (detail::FloatPi / detail::FloatDegree180);
}

inline float SinTable(AngleIndex angleIndex) {
    uint32_t sampleTableIndex = (angleIndex >> 24) & 0xFF;
    float rest = static_cast<float>(angleIndex & 0xFFFFFF) / 0x1000000;
    const detail::SinCosSample* table = &detail::SinCosSampleTable[sampleTableIndex];
    return table->sinValue + table->sinDelta * rest;
}

inline float CosTable(AngleIndex angleIndex) {
    uint32_t sampleTableIndex = (angleIndex >> 24) & 0xFF;
    const detail::SinCosSample* table = &detail::SinCosSampleTable[sampleTableIndex];
    float rest = static_cast<float>(angleIndex & 0xFFFFFF) / 0x1000000;
    return table->cosValue + table->cosDelta * rest;
}

struct SinCosTable {
    float sinValue;
    float cosValue;
};

inline SinCosTable CalcSinCos(float radian) {
    float angleIndex = radian * detail::Float1Divided2Pi;
    angleIndex =
        static_cast<float>(static_cast<int64_t>(angleIndex + (angleIndex >= 0.0f ? 0.5f : -0.5f)));
    float x = radian - angleIndex * detail::Float2Pi;

    float sign = 1.0f;
    if (x > detail::FloatPiDivided2) {
        x = detail::FloatPi - x;
        sign = -1.0f;
    }
    if (x < -detail::FloatPiDivided2) {
        x = -detail::FloatPi - x;
        sign = -sign;
    }

    float x2 = x * x;

    float sinAcc = detail::SinCoefficients[1] - x2 * detail::SinCoefficients[0];
    sinAcc = -detail::SinCoefficients[2] + x2 * sinAcc;
    sinAcc = detail::SinCoefficients[3] + x2 * sinAcc;
    sinAcc = -detail::SinCoefficients[4] + x2 * sinAcc;
    float sinValue = x * (1.0f + x2 * sinAcc);

    float cosAcc = detail::CosCoefficients[1] - x2 * detail::CosCoefficients[0];
    cosAcc = -detail::CosCoefficients[2] + x2 * cosAcc;
    cosAcc = detail::CosCoefficients[3] + x2 * cosAcc;
    cosAcc = -detail::CosCoefficients[4] + x2 * cosAcc;
    float cosValue = (1.0f + x2 * cosAcc) * sign;

    return {sinValue, cosValue};
}

inline void SinCosF32(float* pSin, float* pCos, float radian) {
    AngleIndex angleIndex = RadianToAngleIndex(radian);
    *pSin = SinTable(angleIndex);
    *pCos = CosTable(angleIndex);
}

}  // namespace nn::util