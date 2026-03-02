#pragma once

#include <nn/atk/atk_BiquadFilterCallback.h>

namespace nn::atk::detail {
class BiquadFilterLpf : BiquadFilterCallback {
public:
    constexpr static u32 CoefficientsTableSize = 112;

    static Coefficients CoefficientsTable32000[CoefficientsTableSize];

    ~BiquadFilterLpf() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterLpf) == 0x8);

class BiquadFilterHpf : BiquadFilterCallback {
public:
    constexpr static u32 CoefficientsTableSize = 97;

    static Coefficients CoefficientsTable32000[CoefficientsTableSize];

    ~BiquadFilterHpf() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterHpf) == 0x8);

class BiquadFilterBpf512 : BiquadFilterCallback {
public:
    constexpr static u32 CoefficientsTableSize = 122;

    static Coefficients CoefficientsTable32000[CoefficientsTableSize];

    ~BiquadFilterBpf512() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf512) == 0x8);

class BiquadFilterBpf1024 : BiquadFilterCallback {
public:
    constexpr static u32 CoefficientsTableSize = 93;
    
    static Coefficients CoefficientsTable32000[CoefficientsTableSize];

    ~BiquadFilterBpf1024() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf1024) == 0x8);

class BiquadFilterBpf2048 : BiquadFilterCallback {
public:
    constexpr static u32 CoefficientsTableSize = 93;

    static Coefficients CoefficientsTable32000[CoefficientsTableSize];

    ~BiquadFilterBpf2048() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf2048) == 0x8);

class BiquadFilterLpfNw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[BiquadFilterLpf::CoefficientsTableSize];

    ~BiquadFilterLpfNw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterLpfNw4fCompatible48k) == 0x8);

class BiquadFilterHpfNw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[BiquadFilterHpf::CoefficientsTableSize];

    ~BiquadFilterHpfNw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterHpfNw4fCompatible48k) == 0x8);

class BiquadFilterBpf512Nw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[BiquadFilterBpf512::CoefficientsTableSize];

    ~BiquadFilterBpf512Nw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf512Nw4fCompatible48k) == 0x8);

class BiquadFilterBpf1024Nw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[BiquadFilterBpf1024::CoefficientsTableSize];

    ~BiquadFilterBpf1024Nw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf1024Nw4fCompatible48k) == 0x8);

class BiquadFilterBpf2048Nw4fCompatible48k : BiquadFilterCallback {
public:
    static Coefficients CoefficientsTable48000[BiquadFilterBpf2048::CoefficientsTableSize];

    ~BiquadFilterBpf2048Nw4fCompatible48k() override;

    void GetCoefficients(Coefficients* pOutValue, s32 type, f32 value) override;
};
static_assert(sizeof(BiquadFilterBpf2048Nw4fCompatible48k) == 0x8);

namespace driver {
static BiquadFilterLpf BiquadFilterInstanceLpf {};
static BiquadFilterHpf BiquadFilterInstanceHpf {};
static BiquadFilterBpf512 BiquadFilterInstanceBpf512 {};
static BiquadFilterBpf1024 BiquadFilterInstanceBpf1024 {};
static BiquadFilterBpf2048 BiquadFilterInstanceBpf2048 {};
static BiquadFilterLpfNw4fCompatible48k BiquadFilterInstanceLpfNw4fCompatible48k {};
static BiquadFilterHpfNw4fCompatible48k BiquadFilterInstanceHpfNw4fCompatible48k {};
static BiquadFilterBpf512Nw4fCompatible48k BiquadFilterInstanceBpf512Nw4fCompatible48k {};
static BiquadFilterBpf1024Nw4fCompatible48k BiquadFilterInstanceBpf1024Nw4fCompatible48k {};
static BiquadFilterBpf2048Nw4fCompatible48k BiquadFilterInstanceBpf2048Nw4fCompatible48k {};
};
} // namespace nn::atk::detail