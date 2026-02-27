#pragma once

#include <nn/atk/detail/strm/atk_StreamSoundFile.h>

namespace nn::atk::detail {
class IRegionInfoReadable {
public:
    virtual ~IRegionInfoReadable() = default;
    
    virtual bool ReadRegionInfo(StreamSoundFile::RegionInfo* pInfo, 
                                u32 regionIndex) const = 0;
};
static_assert(sizeof(IRegionInfoReadable) == 0x8);
} // namespace nn::atk::detail