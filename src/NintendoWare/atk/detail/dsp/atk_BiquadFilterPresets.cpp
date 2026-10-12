#include <nn/atk/atk_BiquadFilterPresets.h>

#include <nn/atk/fnd/basis/atkfnd_Inlines.h>

namespace nn::atk::detail {

const BiquadFilterLpf::Coefficients
    BiquadFilterLpf::CoefficientsTable32000[BiquadFilterLpf::CoefficientsTableSize]{/*TODO*/};
const BiquadFilterHpf::Coefficients
    BiquadFilterHpf::CoefficientsTable32000[BiquadFilterHpf::CoefficientsTableSize]{/*TODO*/};
const BiquadFilterBpf512::Coefficients
    BiquadFilterBpf512::CoefficientsTable32000[BiquadFilterBpf512::CoefficientsTableSize]{/*TODO*/};
const BiquadFilterBpf1024::Coefficients
    BiquadFilterBpf1024::CoefficientsTable32000[BiquadFilterBpf1024::CoefficientsTableSize]{
        /*TODO*/};
const BiquadFilterLpfNw4fCompatible48k::Coefficients BiquadFilterLpfNw4fCompatible48k::
    CoefficientsTable48000[BiquadFilterLpfNw4fCompatible48k::CoefficientsTableSize]{/*TODO*/};
const BiquadFilterHpfNw4fCompatible48k::Coefficients BiquadFilterHpfNw4fCompatible48k::
    CoefficientsTable48000[BiquadFilterHpfNw4fCompatible48k::CoefficientsTableSize]{/*TODO*/};
const BiquadFilterBpf512Nw4fCompatible48k::Coefficients BiquadFilterBpf512Nw4fCompatible48k::
    CoefficientsTable48000[BiquadFilterBpf512Nw4fCompatible48k::CoefficientsTableSize]{/*TODO*/};
const BiquadFilterBpf1024Nw4fCompatible48k::Coefficients BiquadFilterBpf1024Nw4fCompatible48k::
    CoefficientsTable48000[BiquadFilterBpf1024Nw4fCompatible48k::CoefficientsTableSize]{/*TODO*/};

void BiquadFilterLpf::GetCoefficients(Coefficients* pOutValue, [[maybe_unused]] int type,
                                      float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>(value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable32000[coefIndex];
}

void BiquadFilterHpf::GetCoefficients(Coefficients* pOutValue, [[maybe_unused]] int type,
                                      float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>(value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable32000[coefIndex];
}

void BiquadFilterBpf512::GetCoefficients(Coefficients* pOutValue, [[maybe_unused]] int type,
                                         float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>(value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable32000[coefIndex];
}

void BiquadFilterBpf1024::GetCoefficients(Coefficients* pOutValue, [[maybe_unused]] int type,
                                          float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>((2.0f - value) * value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable32000[coefIndex];
}

void BiquadFilterBpf2048::GetCoefficients(Coefficients* pOutValue, [[maybe_unused]] int type,
                                          float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>((2.0f - value) * value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable32000[coefIndex];
}

void BiquadFilterLpfNw4fCompatible48k::GetCoefficients(Coefficients* pOutValue,
                                                       [[maybe_unused]] int type,
                                                       float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>(value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable48000[coefIndex];
}

void BiquadFilterHpfNw4fCompatible48k::GetCoefficients(Coefficients* pOutValue,
                                                       [[maybe_unused]] int type,
                                                       float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>(value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable48000[coefIndex];
}

void BiquadFilterBpf512Nw4fCompatible48k::GetCoefficients(Coefficients* pOutValue,
                                                          [[maybe_unused]] int type,
                                                          float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>((2.0f - value) * value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable48000[coefIndex];
}

void BiquadFilterBpf1024Nw4fCompatible48k::GetCoefficients(Coefficients* pOutValue,
                                                           [[maybe_unused]] int type,
                                                           float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>((2.0f - value) * value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable48000[coefIndex];
}

void BiquadFilterBpf2048Nw4fCompatible48k::GetCoefficients(Coefficients* pOutValue,
                                                           [[maybe_unused]] int type,
                                                           float value) const {
    int coefSize{CoefficientsTableSize};
    int coefIndex{static_cast<int>((2.0f - value) * value * static_cast<float>(coefSize - 1))};

    coefIndex = fnd::Clamp(coefIndex, 0, coefSize - 1);
    *pOutValue = CoefficientsTable48000[coefIndex];
}

}  // namespace nn::atk::detail
