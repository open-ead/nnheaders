#include <nn/atk/atk_DspadpcmReader.h>

#include <cstring>

#include <nn/util/util_BytePtr.h>

namespace nn::atk::detail {
namespace {
struct InternalDSPADPCMInfo {
    u32 sampleCount;
    u32 adpcmNibbleCount;
    u32 sampleRate;
    u16 loopFlag;
    u16 format;
    u32 sa;
    u32 ea;
    u32 ca;
    u16 coef[16];
    u16 gain;
    u16 ps;
    u16 yn1;
    u16 yn2;
    u16 lps;
    u16 lyn1;
    u16 lyn2;
    u16 pad[11];
};
static_assert(sizeof(InternalDSPADPCMInfo) == 0x60);
} // anonymous namespace

DspadpcmReader::DspadpcmReader() = default;

bool DspadpcmReader::ReadWaveInfo(WaveInfo* info) const {
    const InternalDSPADPCMInfo& data {*util::ConstBytePtr(m_pDspadpcmData).Get<InternalDSPADPCMInfo>()};

    info->sampleFormat = SampleFormat_DspAdpcm;
    info->loopFlag = false;
    info->channelCount = 1;
    info->sampleRate = data.sampleRate;
    info->loopStartFrame = 0;
    info->loopEndFrame = data.sampleCount;
    
    info->channelParam[0].dataAddress = util::ConstBytePtr(m_pDspadpcmData, sizeof(InternalDSPADPCMInfo)).Get();
    std::memcpy(info->channelParam[0].adpcmParam.coef, data.coef, sizeof(data.coef));
    int yn1 = data.yn1;
    info->channelParam[0].adpcmParam.predScale = data.ps;
    info->channelParam[0].adpcmParam.yn1 = yn1;
    info->channelParam[0].adpcmParam.yn2 = data.yn2;
    return true;
}
} // namespace nn::atk::detail