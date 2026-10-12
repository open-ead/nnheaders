#pragma once

#include <nn/atk/atk_BiquadFilterCallback.h>

namespace nn::atk::detail {

class BiquadFilterLpf : public BiquadFilterCallback {
public:
    BiquadFilterLpf() = default;
    ~BiquadFilterLpf() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{112};
    static const Coefficients CoefficientsTable32000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterLpf) == 0x8);

class BiquadFilterHpf : public BiquadFilterCallback {
public:
    BiquadFilterHpf() = default;
    ~BiquadFilterHpf() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{97};
    static const Coefficients CoefficientsTable32000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterHpf) == 0x8);

class BiquadFilterBpf512 : public BiquadFilterCallback {
public:
    BiquadFilterBpf512() = default;
    ~BiquadFilterBpf512() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{122};
    static const Coefficients CoefficientsTable32000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterBpf512) == 0x8);

class BiquadFilterBpf1024 : public BiquadFilterCallback {
public:
    BiquadFilterBpf1024() = default;
    ~BiquadFilterBpf1024() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{93};
    static const Coefficients CoefficientsTable32000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterBpf1024) == 0x8);

class BiquadFilterBpf2048 : public BiquadFilterCallback {
public:
    BiquadFilterBpf2048() = default;
    ~BiquadFilterBpf2048() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{93};
    static const Coefficients CoefficientsTable32000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterBpf2048) == 0x8);

class BiquadFilterLpfNw4fCompatible48k : public BiquadFilterCallback {
public:
    BiquadFilterLpfNw4fCompatible48k() = default;
    ~BiquadFilterLpfNw4fCompatible48k() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{112};
    static const Coefficients CoefficientsTable48000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterLpfNw4fCompatible48k) == 0x8);

class BiquadFilterHpfNw4fCompatible48k : public BiquadFilterCallback {
public:
    BiquadFilterHpfNw4fCompatible48k() = default;
    ~BiquadFilterHpfNw4fCompatible48k() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{97};
    static const Coefficients CoefficientsTable48000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterHpfNw4fCompatible48k) == 0x8);

class BiquadFilterBpf512Nw4fCompatible48k : public BiquadFilterCallback {
public:
    BiquadFilterBpf512Nw4fCompatible48k() = default;
    ~BiquadFilterBpf512Nw4fCompatible48k() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{122};
    static const Coefficients CoefficientsTable48000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterBpf512Nw4fCompatible48k) == 0x8);

class BiquadFilterBpf1024Nw4fCompatible48k : public BiquadFilterCallback {
public:
    BiquadFilterBpf1024Nw4fCompatible48k() = default;
    ~BiquadFilterBpf1024Nw4fCompatible48k() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{93};
    static const Coefficients CoefficientsTable48000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterBpf1024Nw4fCompatible48k) == 0x8);

class BiquadFilterBpf2048Nw4fCompatible48k : public BiquadFilterCallback {
public:
    BiquadFilterBpf2048Nw4fCompatible48k() = default;
    ~BiquadFilterBpf2048Nw4fCompatible48k() override = default;

    void GetCoefficients(Coefficients* pOutValue, int type, float value) const override;

private:
    static const int CoefficientsTableSize{93};
    static const Coefficients CoefficientsTable48000[CoefficientsTableSize];
};
static_assert(sizeof(BiquadFilterBpf2048Nw4fCompatible48k) == 0x8);

namespace driver {

static BiquadFilterLpf BiquadFilterInstanceLpf{};
static BiquadFilterHpf BiquadFilterInstanceHpf{};
static BiquadFilterBpf512 BiquadFilterInstanceBpf512{};
static BiquadFilterBpf1024 BiquadFilterInstanceBpf1024{};
static BiquadFilterBpf2048 BiquadFilterInstanceBpf2048{};
static BiquadFilterLpfNw4fCompatible48k BiquadFilterInstanceLpfNw4fCompatible48k{};
static BiquadFilterHpfNw4fCompatible48k BiquadFilterInstanceHpfNw4fCompatible48k{};
static BiquadFilterBpf512Nw4fCompatible48k BiquadFilterInstanceBpf512Nw4fCompatible48k{};
static BiquadFilterBpf1024Nw4fCompatible48k BiquadFilterInstanceBpf1024Nw4fCompatible48k{};
static BiquadFilterBpf2048Nw4fCompatible48k BiquadFilterInstanceBpf2048Nw4fCompatible48k{};

};  // namespace driver
}  // namespace nn::atk::detail
