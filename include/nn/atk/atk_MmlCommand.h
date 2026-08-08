#pragma once

namespace nn::atk::detail::driver {
class MmlCommand {
public:
    enum Mml {
        Mml_Wait = 0x80,
        Mml_Prg,

        Mml_OpenTrack = 0x88,
        Mml_Jump,
        Mml_Call,

        Mml_Random = 0xa0,
        Mml_Variable,
        Mml_If,
        Mml_Time,
        Mml_TimeRandom,
        Mml_TimeVariable,

        Mml_Timebase = 0xb0,
        Mml_EnvHold,
        Mml_Monophonic,
        Mml_VelocityRange,
        Mml_BiquadType,
        Mml_BiquadValue,
        Mml_BankSelect,

        Mml_ModPhase = 0xbd,
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
        
        Mml_SweepPitch = 0xe3,
        Mml_ModPeriod,

        Mml_ExCommand = 0xf0,
        
        Mml_EnvReset = 0xfb,
        Mml_LoopEnd,
        Mml_Ret,
        Mml_AllocTrack,
        Mml_Fin,
    };
};
} // namespace nn::atk::detail::driver