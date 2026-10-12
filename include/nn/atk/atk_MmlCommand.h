#pragma once

namespace nn::atk::detail::driver {

class MmlCommand {
public:
    enum Mml {
        Mml_Wait = 128,
        Mml_Prg,

        Mml_OpenTrack = 136,
        Mml_Jump,
        Mml_Call,

        Mml_Random = 160,
        Mml_Variable,
        Mml_If,
        Mml_Time,
        Mml_TimeRandom,
        Mml_TimeVariable,

        Mml_Timebase = 176,
        Mml_EnvHold,
        Mml_Monophonic,
        Mml_VelocityRange,
        Mml_BiquadType,
        Mml_BiquadValue,
        Mml_BankSelect,

        Mml_ModPhase = 189,
        Mml_ModCurve,
        Mml_FrontBypass,
        Mml_Pan,
        Mml_Volume,
        Mml_MainVolume,
        Mml_Transpose,
        Mml_PitchBend,
        Mml_BendRange,
        Mml_Prio,
        Mml_NoteWait,
        Mml_Tie,
        Mml_Porta,
        Mml_ModDepth,
        Mml_ModSpeed,
        Mml_ModType,
        Mml_ModRange,
        Mml_PortaSw,
        Mml_PortaTime,
        Mml_Attack,
        Mml_Decay,
        Mml_Sustain,
        Mml_Release,
        Mml_LoopStart,
        Mml_Volume2,
        Mml_Printvar,
        Mml_SurroundPan,
        Mml_LpfCutoff,
        Mml_FxsendA,
        Mml_FxsendB,
        Mml_Mainsend,
        Mml_InitPan,
        Mml_Mute,
        Mml_FxsendC,
        Mml_Damper,
        Mml_ModDelay,
        Mml_Tempo,

        Mml_SweepPitch = 227,
        Mml_ModPeriod,

        Mml_ExCommand = 240,

        Mml_EnvReset = 251,
        Mml_LoopEnd,
        Mml_Ret,
        Mml_AllocTrack,
        Mml_Fin,
    };
};

}  // namespace nn::atk::detail::driver
