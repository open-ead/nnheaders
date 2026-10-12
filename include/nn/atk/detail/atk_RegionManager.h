#pragma once

#include <nn/types.h>

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/atk_Config.h>
#include <nn/atk/detail/atk_IRegionInfoReadable.h>

namespace nn::atk {

enum StreamRegionCallbackResult {
    StreamRegionCallbackResult_Finish,
    StreamRegionCallbackResult_Continue,
};

struct StreamRegionCallbackParam {
    int regionNo;
#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
    int _4;
    int _8;
    int _c;
    int _10;
#else
    char regionName[64];
    bool isRegionNameEnabled;
#endif
    int regionCount;
    detail::IRegionInfoReadable* pRegionInfoReader;
};
#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
static_assert(sizeof(StreamRegionCallbackParam) == 0x20);
#else
static_assert(sizeof(StreamRegionCallbackParam) == 0x58);
#endif

using StreamRegionCallback = StreamRegionCallbackResult (*)(StreamRegionCallbackParam*, void*);

namespace detail {

struct StreamDataInfoDetail;

class RegionManager {
public:
    struct Region {
        position_t current{0};
        position_t begin{0};
        position_t end{0};
#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
        bool isEnabled;
#endif

        Region() = default;

        bool IsIn(position_t value) const { return current + value < end; }

        bool IsInWithBorder(position_t value) const { return current + value <= end; }

        bool IsEnd() const { return current == end; }

        size_t Rest() const { return end - current; }
    };

    RegionManager() = default;

    void Initialize();
    bool InitializeRegion(IRegionInfoReadable* pRegionReader,
                          StreamDataInfoDetail* pStreamDataInfo);

    bool ChangeRegion(int currentRegionNo, IRegionInfoReadable* pRegionReader,
                      StreamDataInfoDetail* pStreamDataInfo);

    bool TryMoveNextRegion(IRegionInfoReadable* pRegionReader,
                           StreamDataInfoDetail* pStreamDataInfo);

    void SetPosition(position_t position);
    void AddPosition(position_t position);

    const Region& GetCurrentRegion() const { return m_CurrentRegion; }

    void SetStartOffsetFrame(position_t startOffsetFrame) {
        m_AdpcmContextForStartOffsetFrame = startOffsetFrame;
    }

    position_t GetStartOffsetFrame() const { return m_AdpcmContextForStartOffsetFrame; }

    AdpcmContext& GetAdpcmContextForStartOffset(int channelIndex) {
        return m_AdpcmContextForStartOffset[channelIndex];
    }
    const AdpcmContext& GetAdpcmContextForStartOffset(int channelIndex) const {
        return m_AdpcmContextForStartOffset[channelIndex];
    }

    bool IsInFirstRegion() const;

    void SetRegionCallback(const StreamRegionCallback& function, void* argument) {
        m_StreamRegionCallbackFunc = function;
        m_StreamRegionCallbackArg = argument;
    }

    bool IsPreparedForRegionJump() const;

#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
    void SetRegionInfo(int regionNo, IRegionInfoReadable* pRegionReader,
                       StreamDataInfoDetail* pStreamDataInfo);
#else
    void SetRegionInfo(const StreamSoundFile::RegionInfo* pRegionInfo,
                       const StreamDataInfoDetail* pStreamDataInfo);
#endif

private:
    bool m_IsRegionInfoEnabled;
    bool m_IsRegionIndexCheckEnabled;
#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
    bool m_IsRegionInitialized;
    bool m_IsCurrentRegionNameEnabled;
#endif
    StreamRegionCallback m_StreamRegionCallbackFunc;
    void* m_StreamRegionCallbackArg;
    int m_CurrentRegionNo;
#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
    const char* m_pCurrentRegionName;
#endif
    Region m_CurrentRegion;
    position_t m_AdpcmContextForStartOffsetFrame;
    AdpcmContext m_AdpcmContextForStartOffset[StreamChannelCount];
#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
    char m_CurrentRegionName[64];
#endif
};
#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
static_assert(sizeof(RegionManager) == 0x440);
#else
static_assert(sizeof(RegionManager) == 0x4c0);
#endif

}  // namespace detail
}  // namespace nn::atk
