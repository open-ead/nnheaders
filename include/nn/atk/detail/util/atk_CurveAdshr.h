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

    constexpr static f32 VolumeInit = -90.4;
    constexpr static f32 AttackInit = 127;
    constexpr static u32 HoldInit = 0;
    constexpr static f32 DecayInit = 127;
    constexpr static f32 SustainInit = 127;
    constexpr static f32 ReleaseInit = 127;
    
    constexpr static u32 DecibelSquareTableSize = 128;
    constexpr static u32 CalcDecibelScaleMax = 127;

    constexpr static u16 DecibelSquareTable[DecibelSquareTableSize] = {
        64813, 64814, 64815, 64885, 64935, 64974, 65006, 65033,
        65056, 65076, 65094, 65111, 65126, 65140, 65153, 65165,
        65176, 65187, 65197, 65206, 65215, 65223, 65231, 65239,
        65247, 65254, 65260, 65267, 65273, 65279, 65285, 65291,
        65297, 65302, 65307, 65312, 65317, 65322, 65326, 65331,
        65335, 65340, 65344, 65348, 65352, 65356, 65360, 65363,
        65367, 65371, 65374, 65378, 65381, 65384, 65387, 65391,
        65394, 65397, 65400, 65403, 65406, 65409, 65411, 65414,
        65417, 65420, 65422, 65425, 65427, 65430, 65433, 65435,
        65437, 65440, 65442, 65445, 65447, 65449, 65451, 65454,
        65456, 65458, 65460, 65462, 65464, 65466, 65468, 65470,
        65472, 65474, 65476, 65478, 65480, 65482, 65484, 65486,
        65487, 65489, 65491, 65493, 65494, 65496, 65498, 65500,
        65501, 65503, 65505, 65506, 65508, 65509, 65511, 65513,
        65514, 65516, 65517, 65519, 65520, 65522, 65523, 65525,
        65526, 65528, 65529, 65530, 65532, 65533, 65535,     0,
    };

    CurveAdshr();

    void Initialize(f32 initDecibel);

    void SetAttack(s32 attack);
    void SetHold(s32 hold);
    void SetDecay(s32 decay);
    void SetSustain(s32 sustain);
    void SetRelease(s32 release);

    void Reset(f32 initDecibel);

    f32 GetValue() const;

    void Update(s32 msec);

    static u16 CalcDecibelSquare(s32);
    static f32 CalcRelease(s32);

private:
    Status m_Status{Status_Attack};
    f32 m_Value{-904.0};
    f32 m_Decay{65535.0};
    f32 m_Release{65535.0};
    f32 m_Attack{0.0};
    u16 m_Hold{0};
    u16 m_HoldCounter;
    u8 m_Sustain{127};
    u8 m_Padding[3];
};
static_assert(sizeof(CurveAdshr) == 0x1c);
} // namespace nn::atk::detail