#pragma once

#include <atomic>

#include <nn/os.h>
#include <nn/audio/audio_Common.h>
#include <nn/util/util_IntrusiveList.h>

namespace nn::atk {
struct TimeRange {
    os::Tick begin;
    os::Tick end;
};
static_assert(sizeof(TimeRange) == 0x10);

struct SoundProfile {
    TimeRange nwFrameProcess;
    TimeRange mainMixProcess;
    TimeRange finalMixProcess;
    TimeRange voiceProcess;
    TimeRange sinkProcess;
    TimeRange circularBufferSinkProcess;
    TimeRange rendererFrameProcess;
    u32 totalVoiceCount;
    u32 rendererVoiceCount;
    u32 nwVoiceCount;
    u64 nwFrameProcessTick;
    TimeRange _additionalSubMixProcess;
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    TimeRange _voiceProcessTable[96];
    audio::NodeId _voiceIdTable[96];
#else
    TimeRange _voiceProcessTable[192];
    audio::NodeId _voiceIdTable[192];
#endif
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(SoundProfile) == 0x818);
#else
static_assert(sizeof(SoundProfile) == 0xf98);
#endif

struct SoundThreadUpdateProfile {
    TimeRange soundThreadProcess;
    TimeRange _updateLowLevelVoiceProcess;
    TimeRange _updateRendererProcess;
    TimeRange _waitRendererEventProcess;
    TimeRange _userEffectFrameProcess;
    TimeRange _frameProcess;
};
static_assert(sizeof(SoundThreadUpdateProfile) == 0x60);

class ProfileReader {
public:
    ProfileReader();

    std::size_t Read(SoundProfile*, s32);

    void Record(const SoundProfile& src);

    util::IntrusiveListNode m_Link;

private:
    SoundProfile m_ProfileBuffer[32];
    s32 m_ProfileBufferRead;
    s32 m_ProfileBufferWrite;
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(ProfileReader) == 0x10318);
#else
static_assert(sizeof(ProfileReader) == 0x1f318);
#endif

using ProfileReaderList = util::IntrusiveList<ProfileReader, 
                            util::IntrusiveListMemberNodeTraits<ProfileReader, 
                                &ProfileReader::m_Link>>;

struct TaskProfile;

template <typename T>
class AtkProfileReader {
public:
    util::IntrusiveListNode m_List;

private:
    bool m_IsInitialized;
    TaskProfile* m_pProfile;
    s32 m_ProfileCount;
    s32 m_RecordIndex;
    s32 m_ReadIndex;
    std::atomic_int m_ReadableCount;
};

using SoundThreadUpdateProfileReader = AtkProfileReader<SoundThreadUpdateProfile>;
using SoundThreadUpdateProfileReaderList = util::IntrusiveList<SoundThreadUpdateProfileReader, 
                                            util::IntrusiveListMemberNodeTraits<SoundThreadUpdateProfileReader, 
                                                &SoundThreadUpdateProfileReader::m_List>>;
} // namespace nn::atk