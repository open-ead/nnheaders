#include <nn/atk/atk_CurveAdshr.h>

namespace nn::atk::detail {

void CurveAdshr::Reset(float initDecibel) {
    m_Value = initDecibel * 10;
    m_Status = Status_Attack;
}

void CurveAdshr::SetDecay(int decay) {
    m_Decay = CalcRelease(decay);
}

float CurveAdshr::CalcRelease(int release) {
    if (release == ReleaseInit)
        return 4.0f * 128 * 128 - 1;

    if (release == ReleaseInit - 1)
        return 120 / 5.0f;

    if (release < 50)
        return static_cast<float>((release * 2) + 1) / 128.0f / 5.0f;

    return (60.0f / static_cast<float>(126 - release)) / 5.0f;
}

}  // namespace nn::atk::detail