#include <nn/atk/atk_CurveAdshr.h>

namespace nn::atk::detail {

void CurveAdshr::Reset(float initDecibel) {
    m_Value = initDecibel * 10;
    m_Status = Status_Attack;
}

}  // namespace nn::atk::detail