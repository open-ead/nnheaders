#include <nn/atk/atk_Bank.h>

namespace {
const float VelocityMaxR{1.0f / 127.0f};

const float InstrumentVolumeCriterionR{1.0f / 127.0f};
}

namespace nn::atk::detail::driver {

Bank::Bank() = default;

Bank::~Bank() = default;

float Bank::CalcChannelVelocityVolume(u8 velocity) {
    return static_cast<float>(velocity) * VelocityMaxR;
}

}  // namespace nn::atk::detail::driver
