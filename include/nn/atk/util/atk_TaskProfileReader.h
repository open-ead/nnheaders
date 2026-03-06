#pragma once

#include <nn/time.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/detail/atk_Config.h>
#include <nn/atk/detail/atk_IStreamDataDecoder.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/util/atk_ProfileReader.h>

namespace nn::atk {

namespace detail::driver {
class StreamSoundPlayer;
} // namespace nn::atk::detail::driver 

struct TaskProfile {
    enum TaskProfileType {
        TaskProfileType_LoadStreamBlock,
        TaskProfileType_LoadOpusStreamBlock,
    };

    class LoadStreamBlock {
    public:
        TimeSpan GetTotalTime() const;
        u64 GetBeginTick() const;
        u64 GetEndTick() const;

        f32 GetRemainingCachePercentage() const;
        size_t GetCachedLength() const;

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        detail::driver::StreamSoundPlayer* GetStreamSoundPlayer() const;
#endif

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
        void SetTick(const os::Tick& beginTick, const os::Tick& endTick);
#else
        void SetData(const os::Tick& beginTick, const os::Tick& endTick, 
                     const detail::IStreamDataDecoder::CacheProfile& cacheProfile);
#endif

    private:
        u64 m_BeginTick;
        u64 m_EndTick;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        position_t m_CacheStartPosition;
        size_t m_CachedLength;
        position_t m_CacheCurrentPosition;
        detail::driver::StreamSoundPlayer* m_pPlayer;
#endif
    };
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    static_assert(sizeof(LoadStreamBlock) == 0x10);
#else
    static_assert(sizeof(LoadStreamBlock) == 0x30);
#endif

    class LoadOpusStreamBlock {
    public:
        TimeSpan GetTotalTime() const;
        u64 GetBeginTick() const;
        u64 GetEndTick() const;

        f32 GetRemainingCachePercentage() const;
        size_t GetCachedLength() const;

        TimeSpan GetDecodeTime() const;
        s32 GetDecodedSampleCount() const;

        TimeSpan GetFsAccessTime();
        size_t GetFsReadSize();

#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        detail::driver::StreamSoundPlayer* GetStreamSoundPlayer() const;
#endif

#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
        void SetData(const os::Tick& beginTick, const os::Tick& endTick, 
                     detail::IStreamDataDecoder::DecodeProfile* decodeProfile);
#else
        void SetData(const os::Tick& beginTick, const os::Tick& endTick, 
                     const detail::IStreamDataDecoder::DecodeProfile& decodeProfile, 
                     const detail::IStreamDataDecoder::CacheProfile& cacheProfile);
#endif

    private:
        u64 m_BeginTick;
        u64 m_EndTick;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        position_t m_CacheStartPosition;
        size_t m_CachedLength;
        position_t m_CacheCurrentPosition;
#endif
        u64 m_DecodeTick;
        u64 m_FsAccessTick;
        size_t m_FsReadSize;
        s32 m_DecodedSampleCount;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        detail::driver::StreamSoundPlayer* m_pPlayer;
#endif
    };
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    static_assert(sizeof(LoadOpusStreamBlock) == 0x30);
#else
    static_assert(sizeof(LoadOpusStreamBlock) == 0x50);
#endif

    TaskProfileType type;
    union {
        LoadStreamBlock loadStreamBlock;
        LoadOpusStreamBlock loadOpusStreamBlock;
    };
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(TaskProfile) == 0x38);
#else
static_assert(sizeof(TaskProfile) == 0x58);
#endif

using TaskProfileReader = AtkProfileReader<TaskProfile>;

class TaskProfileLogger {
public:
    using TaskProfileReaderList = util::IntrusiveList<TaskProfileReader, 
                                    util::IntrusiveListMemberNodeTraits<TaskProfileReader, 
                                        &TaskProfileReader::m_List>>;
    
    TaskProfileLogger();

    void Record(const TaskProfile& profile);

    void RegisterReader(TaskProfileReader& profileReader);
    void UnregisterReader(const TaskProfileReader& profileReader);
    
    void SetProfilingEnabled(bool isEnabledProfiling);

    void Finalize();

private:
    TaskProfileReaderList m_List;
    detail::fnd::CriticalSection m_Lock;
    bool m_IsProfilingEnabled;
};
static_assert(sizeof(TaskProfileLogger) == 0x38);
} // namespace nn::atk