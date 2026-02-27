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

        void SetTick(const os::Tick& beginTick, const os::Tick& endTick);
        void SetData(os::Tick* beginTick, os::Tick* endTick, 
                     detail::IStreamDataDecoder::CacheProfile* cacheProfile);

    private:
        u64 m_BeginTick;
        u64 m_EndTick;
        position_t m_CacheStartPosition;
        std::size_t m_CachedLength;
        position_t m_CacheCurrentPosition;
        detail::driver::StreamSoundPlayer* m_pPlayer;
    };
    static_assert(sizeof(LoadStreamBlock) == 0x30);

    class LoadOpusStreamBlock {
    public:
        TimeSpan GetTotalTime() const;
        u64 GetBeginTick() const;
        u64 GetEndTick() const;
        TimeSpan GetDecodeTime() const;
        s32 GetDecodedSampleCount() const;
        TimeSpan GetFsAccessTime();
        std::size_t GetFsReadSize();
    
        void SetData(os::Tick* beginTick, os::Tick* endTick, 
                     detail::IStreamDataDecoder::DecodeProfile* decodeProfile);
        void SetData(os::Tick* beginTick, os::Tick* endTick, 
                     detail::IStreamDataDecoder::DecodeProfile* decodeProfile, 
                     detail::IStreamDataDecoder::CacheProfile* cacheProfile);

    private:
        u64 m_BeginTick;
        u64 m_EndTick;
        position_t m_CacheStartPosition;
        std::size_t m_CachedLength;
        position_t m_CacheCurrentPosition;
        u64 m_DecodeTick;
        u64 m_FsAccessTick;
        std::size_t m_FsReadSize;
        s32 m_DecodedSampleCount;
        detail::driver::StreamSoundPlayer* m_pPlayer;
    };
    static_assert(sizeof(LoadOpusStreamBlock) == 0x50);

    TaskProfileType type;
    union {
        LoadStreamBlock loadStreamBlock;
        LoadOpusStreamBlock loadOpusStreamBlock;
    };
};
static_assert(sizeof(TaskProfile) == 0x58);

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