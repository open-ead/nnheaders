#pragma once

namespace nn::util::detail {
// todo: figure out where to put these
extern float SinCoefficients[5];
extern float CosCoefficients[5];
extern float AtanCoefficients[8];

extern unsigned int AngleIndexHalfRound;
extern float FloatPiDivided2;
extern float Float1Divided2Pi;
extern float FloatPi;
extern float Float2Pi;
extern float FloatDegree180;
extern float FloatQuaternionEpsilon;
extern float FloatUlp;

extern SinCosSample SinCosSampleTable[4];

// todo:
inline float ModPi(float x) {
    float factor;
    float roundedQuotient;
}

struct SinCosSample {
    float cosValue;
    float sinValue;
    float cosDelta;
    float sinDelta;
};

};  // namespace nn::util::detail