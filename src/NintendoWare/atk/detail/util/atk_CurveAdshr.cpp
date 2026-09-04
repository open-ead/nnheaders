#include <nn/atk/atk_CurveAdshr.h>

namespace nn::atk::detail {

void CurveAdshr::Reset(float initDecibel) {
    m_Value = initDecibel * 10;
    m_Status = Status_Attack;
}

void CurveAdshr::SetAttack(int attack) {
    // Table to optimize calculations
    // More info here: https://github.com/stupidestmodder/TuneBloom/blob/master/TuneBloom/src/snd-ply/CurveAdshr.cpp#L127
    static const float attackTable[DecibelSquareTableSize] = {
        0.99921750, 0.99843258, 0.99764520, 0.99685530, 
        0.99606287, 0.99526790, 0.99447040, 0.99367040, 
        0.99286770, 0.99206250, 0.99125460, 0.99044410, 
        0.98963088, 0.98881510, 0.98799650, 0.98717520, 
        0.98635119, 0.98552440, 0.98469490, 0.98386250,
        0.98302728, 0.98218930, 0.98134828, 0.98050450, 
        0.97965780, 0.97880810, 0.97795550, 0.97709990, 
        0.97624129, 0.97537970, 0.97451500, 0.97364718, 
        0.97277629, 0.97190230, 0.97102510, 0.97014480, 
        0.96926120, 0.96837437, 0.96748440, 0.96659100,
        0.96569440, 0.96479440, 0.96389100, 0.96298420, 
        0.96207398, 0.96116040, 0.96024328, 0.95932257, 
        0.95839840, 0.95747060, 0.95653920, 0.95560420, 
        0.95466548, 0.95372307, 0.95277690, 0.95182699, 
        0.95087320, 0.94991570, 0.94895420, 0.94798880,
        0.94701950, 0.94604617, 0.94506890, 0.94408750, 
        0.94310200, 0.94211239, 0.94111860, 0.94012058, 
        0.93911839, 0.93811178, 0.93710089, 0.93608558, 
        0.93506590, 0.93404168, 0.93301308, 0.93197978, 
        0.93094200, 0.92989950, 0.92885230, 0.92780040, 
        0.92674360, 0.92568210, 0.92461560, 0.92354420, 
        0.92246780, 0.92138640, 0.92029980, 0.91920810, 
        0.91811120, 0.91700910, 0.91590160, 0.91478870, 
        0.91367030, 0.91254650, 0.91141710, 0.91028208, 
        0.90914140, 0.90799490, 0.90684270, 0.90568447,
        0.90452039, 0.90335017, 0.90217400, 0.90099160, 
        0.89980290, 0.89860800, 0.89740658, 0.89619880, 
        0.89498440, 0.89005989, 0.88246220, 0.87592470, 
        0.86918610, 0.86364060, 0.85357880, 0.84301889, 
        0.82861350, 0.81490988, 0.80021720, 0.77806628,
        0.75547498, 0.72421250, 0.68282390, 0.63291690, 
        0.55921350, 0.45514110, 0.32987699, 0.00000000 
    };

    m_Attack = attackTable[attack];
}

void CurveAdshr::SetHold(int hold) {
    m_Hold = static_cast<u16>((hold + 1) * (hold + 1) / 4);
}

void CurveAdshr::SetDecay(int decay) {
    m_Decay = CalcRelease(decay);
}

void CurveAdshr::SetSustain(int sustain) {
    m_Sustain = sustain;
}

void CurveAdshr::SetRelease(int release) {
    m_Release = CalcRelease(release);
}

float CurveAdshr::CalcRelease(int release) {
    if (release == 127)
        return 4.0f * 128 * 128 - 1;

    if (release == 126)
        return 120 / 5.0f;

    if (release < 50)
        return static_cast<float>((release * 2) + 1) / 128.0f / 5.0f;

    return (60.0f / static_cast<float>(126 - release)) / 5.0f;
}

}  // namespace nn::atk::detail