#pragma once

#include <nn/atk/atk_BiquadFilterCallback.h>

namespace nn::atk::detail {
class BiquadFilterLpf : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable32000[112];

    ~BiquadFilterLpf() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterLpf) == 0x8);

class BiquadFilterHpf : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable32000[97];

    ~BiquadFilterHpf() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterHpf) == 0x8);

class BiquadFilterBpf512 : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable32000[122];

    ~BiquadFilterBpf512() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf512) == 0x8);

class BiquadFilterBpf1024 : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable32000[93];

    ~BiquadFilterBpf1024() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf1024) == 0x8);

class BiquadFilterBpf2048 : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable32000[93];

    ~BiquadFilterBpf2048() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf2048) == 0x8);

class BiquadFilterLpfNw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[112];

    ~BiquadFilterLpfNw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterLpfNw4fCompatible48k) == 0x8);

class BiquadFilterHpfNw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[97];

    ~BiquadFilterHpfNw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterHpfNw4fCompatible48k) == 0x8);

class BiquadFilterBpf512Nw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[122];

    ~BiquadFilterBpf512Nw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf512Nw4fCompatible48k) == 0x8);

class BiquadFilterBpf1024Nw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[93];

    ~BiquadFilterBpf1024Nw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf1024Nw4fCompatible48k) == 0x8);

class BiquadFilterBpf2048Nw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[93];

    ~BiquadFilterBpf2048Nw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf2048Nw4fCompatible48k) == 0x8);
} // namespace nn::atk::detail