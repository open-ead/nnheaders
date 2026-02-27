#pragma once

#include <nn/atk/detail/seq/atk_MmlSequenceTrack.h>

namespace nn::atk::detail::driver {
class MmlParser {
public:
    enum SeqArgType {
        SeqArgType_None,
        SeqArgType_U8,
        SeqArgType_S16,
        SeqArgType_Vmidi,
        SeqArgType_Random,
        SeqArgType_Variable,
    };

    MmlParser();
    virtual ~MmlParser();

    SequenceTrack::ParseResult Parse(MmlSequenceTrack* track, bool doNoteOn) const;

    s32 ReadArg(const u8** ptr, SequenceSoundPlayer* player, SequenceTrack* track, 
                SeqArgType argType) const;
    
    s32 Read24(const u8** ptr) const;
    s16 Read16(const u8** ptr) const;

    void CommandProc(MmlSequenceTrack* track, u32 command, s32 commandArg1, s32 commandArg2) const;

    s16* GetVariablePtr(SequenceSoundPlayer* player, SequenceTrack* track, s32 varNo) const;

    void NoteOnCommandProc(MmlSequenceTrack* track, s32 key, s32 velocity, s32 length, bool tieFlag) const;

    s16 ReadVar(const u8** ptr) const;

    static u32 ParseAllocTrack(const void* baseAddress, u32 seqOffset, u32* allocTrack);
};
static_assert(sizeof(MmlParser) == 0x8);
} // namespace nn::atk::detail::driver