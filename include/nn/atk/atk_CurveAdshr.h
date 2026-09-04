#pragma once

#include <nn/types.h>

namespace nn::atk::detail {

class CurveAdshr {
public:
    enum Status {
        Status_Attack,
        Status_Hold,
        Status_Decay,
        Status_Sustain,
        Status_Release,
    };

    static const float VolumeInit;
    static const int AttackInit = 127;
    static const int HoldInit = 0;
    static const int DecayInit = 127;
    static const int SustainInit = 127;
    static const int ReleaseInit = 127;

    static const int DecibelSquareTableSize = 128;
    static const int CalcDecibelScaleMax = 127;

    static const s16 DecibelSquareTable[DecibelSquareTableSize];

    CurveAdshr();

    void Initialize(float initDecibel);
    void Reset(float initDecibel);
    void Update(int msec);
    float GetValue() const;

    Status GetStatus() const { return m_Status; }
    void SetStatus(Status status) { m_Status = status; }

    void SetAttack(int attack);
    void SetHold(int hold);
    void SetDecay(int decay);
    void SetSustain(int sustain);
    void SetRelease(int release);

    static float CalcRelease(int release);
    static s16 CalcDecibelSquare(int scale);

private:
    Status m_Status;
    float m_Value;
    float m_Decay;
    float m_Release;
    float m_Attack;
    u16 m_Hold;
    u16 m_HoldCounter;
    u8 m_Sustain;
    u8 m_Padding[3];
};
static_assert(sizeof(CurveAdshr) == 0x1c);

}  // namespace nn::atk::detail