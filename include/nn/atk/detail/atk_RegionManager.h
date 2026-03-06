#pragma once

#include <nn/types.h>

#include <nn/atk/atk_Adpcm.h>
#include <nn/atk/detail/atk_Config.h>
#include <nn/atk/detail/atk_IRegionInfoReadable.h>

namespace nn::atk {
enum StreamRegionCallbackResult {
    StreamRegionCallbackResult_Finish,
    StreamRegionCallbackResult_Continue,
};

struct StreamRegionCallbackParam {
    s32 regionNo;
    char regionName[64];
    bool isRegionNameEnabled;
    s32 regionCount;
    detail::IRegionInfoReadable* pRegionInfoReader;
};
static_assert(sizeof(StreamRegionCallbackParam) == 0x58);

using StreamRegionCallback = StreamRegionCallbackResult(*)(StreamRegionCallbackParam*,void*);

namespace detail {
struct StreamDataInfoDetail;

class RegionManager {
public:
    struct Region {
        position_t current;
        position_t begin;
        position_t end;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
        bool isEnabled;
#endif
    };

    void Initialize();
    bool InitializeRegion(IRegionInfoReadable* pRegionReader, 
                          StreamDataInfoDetail* pStreamDataInfo);

    bool IsPreparedForRegionJump() const;
    
    bool ChangeRegion(s32 currentRegionNo, IRegionInfoReadable* pRegionReader, 
                      StreamDataInfoDetail* pStreamDataInfo);

    bool SetRegionInfo(const StreamSoundFile::RegionInfo* pRegionInfo, 
                       const StreamDataInfoDetail* pStreamDataInfo);

    bool TryMoveNextRegion(IRegionInfoReadable* pRegionReader, 
                           StreamDataInfoDetail* pStreamDataInfo);

    void SetPosition(position_t position);
    void AddPosition(position_t position);

    bool IsInFirstRegion() const;

private:
    bool m_IsRegionInfoEnabled;
    bool m_IsRegionIndexCheckEnabled;
    bool m_IsRegionInitialized;
    bool m_IsCurrentRegionNameEnabled;
    StreamRegionCallback m_StreamRegionCallbackFunc;
    void* m_StreamRegionCallbackArg;
    s32 m_CurrentRegionNo;
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    char* m_pCurrentRegionName;
#endif
    Region m_CurrentRegion;
    position_t m_AdpcmContextForStartOffsetFrame;
    AdpcmContext m_AdpcmContextForStartOffset[16];
#if NN_SDK_VER >= NN_MAKE_VER(4, 0, 0)
    char m_CurrentRegionName[64];
#endif
};
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
static_assert(sizeof(RegionManager) == 0x440);
#else
static_assert(sizeof(RegionManager) == 0x4c0);
#endif
} // namespace nn::atk::detail
} // namespace nn::atk