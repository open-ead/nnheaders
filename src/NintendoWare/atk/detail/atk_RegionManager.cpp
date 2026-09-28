#include <nn/atk/detail/atk_RegionManager.h>

#include <nn/util/util_StringUtil.h>

#include <nn/atk/atk_StreamSoundLoader.h>

namespace nn::atk::detail {

void RegionManager::Initialize() {
    m_IsRegionInfoEnabled = false;
    m_IsRegionIndexCheckEnabled = false;
#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
    m_IsRegionInitialized = false;
#endif
    m_StreamRegionCallbackFunc = nullptr;
    m_StreamRegionCallbackArg = nullptr;
    m_AdpcmContextForStartOffsetFrame = 0xffffffff;
}

bool RegionManager::InitializeRegion(IRegionInfoReadable* pRegionReader,
                                     StreamDataInfoDetail* pStreamDataInfo) {
#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
    if (m_IsRegionIndexCheckEnabled)
        return true;
#else
    if (m_IsRegionInitialized)
        return true;
#endif

    m_CurrentRegionNo = 0;

#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
    m_pCurrentRegionName = nullptr;
    m_IsCurrentRegionNameEnabled = false;
#endif

    StreamSoundFile::RegionInfo regionInfo;
    m_IsRegionInfoEnabled = pRegionReader->ReadRegionInfo(&regionInfo, 0);
    m_IsRegionIndexCheckEnabled = pStreamDataInfo->isRegionIndexCheckEnabled;

    if (IsPreparedForRegionJump()) {
        if (!ChangeRegion(0, pRegionReader, pStreamDataInfo))
            return false;
    } else {
        m_CurrentRegion.begin = 0;
        m_CurrentRegion.end = static_cast<position_t>(pStreamDataInfo->sampleCount);
    }
    m_CurrentRegion.current = m_CurrentRegion.begin;

#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
    m_IsRegionInitialized = true;
#endif

    return true;
}

bool RegionManager::IsPreparedForRegionJump() const {
    if (!m_IsRegionInfoEnabled)
        return false;

    return m_StreamRegionCallbackFunc != nullptr;
}

bool RegionManager::ChangeRegion(int currentRegionNo, IRegionInfoReadable* pRegionReader,
                                 StreamDataInfoDetail* pStreamDataInfo) {
    StreamRegionCallbackParam param;
    param.regionNo = currentRegionNo;
    param.regionCount = pStreamDataInfo->regionCount;
    param.pRegionInfoReader = pRegionReader;

#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
    if (m_pCurrentRegionName != nullptr)
        util::Strlcpy(param.regionName, m_pCurrentRegionName, sizeof(param.regionName));
    else
        std::memset(param.regionName, 0, sizeof(param.regionName));
    param.isRegionNameEnabled = m_IsCurrentRegionNameEnabled;
#endif

    if (m_StreamRegionCallbackFunc(&param, m_StreamRegionCallbackArg) ==
        StreamRegionCallbackResult_Finish)
        return false;

#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
    m_CurrentRegionNo = param.regionNo;
    SetRegionInfo(m_CurrentRegionNo, pRegionReader, pStreamDataInfo);
#else

    if (param.isRegionNameEnabled) {
        int targetRegionIndex{-1};
        for (int i{0}; i < pStreamDataInfo->regionCount; ++i) {
            StreamSoundFile::RegionInfo regionInfo;
            pRegionReader->ReadRegionInfo(&regionInfo, i);
            if (util::Strncmp(regionInfo.regionName, param.regionName, sizeof(param.regionName)) ==
                0) {
                targetRegionIndex = i;
                break;
            }
        }

        if (targetRegionIndex != -1) {
            m_CurrentRegionNo = targetRegionIndex;
            util::Strlcpy(m_CurrentRegionName, param.regionName, sizeof(m_CurrentRegionName));
            m_pCurrentRegionName = m_CurrentRegionName;
            m_IsCurrentRegionNameEnabled = param.isRegionNameEnabled;
        } else {
            m_CurrentRegionNo = param.regionNo;
            m_pCurrentRegionName = nullptr;
            m_IsCurrentRegionNameEnabled = false;
        }

    } else {
        m_CurrentRegionNo = param.regionNo;
        m_pCurrentRegionName = nullptr;
        m_IsCurrentRegionNameEnabled = param.isRegionNameEnabled;
    }

    StreamSoundFile::RegionInfo regionInfo;
    bool result{pRegionReader->ReadRegionInfo(&regionInfo, m_CurrentRegionNo)};

    if (result)
        SetRegionInfo(&regionInfo, pStreamDataInfo);
    else
        SetRegionInfo(nullptr, pStreamDataInfo);

#endif

    return true;
}

#if NN_WARE_VER < NN_MAKE_VER(3, 0, 0)
void RegionManager::SetRegionInfo(int regionNo, IRegionInfoReadable* pRegionReader,
                                  StreamDataInfoDetail* pStreamDataInfo) {
    StreamSoundFile::RegionInfo regionInfo;
    if (!pRegionReader->ReadRegionInfo(&regionInfo, regionNo)) {
        m_CurrentRegion.begin = 0;
        m_CurrentRegion.end = static_cast<position_t>(pStreamDataInfo->sampleCount);
        return;
    }

    m_CurrentRegion.begin = regionInfo.start;
    m_CurrentRegion.end = regionInfo.end;

    if (pStreamDataInfo->sampleFormat == SampleFormat_DspAdpcm) {
        for (int ch{0}; ch < pStreamDataInfo->channelCount; ++ch) {
            m_AdpcmContextForStartOffset[ch].audioAdpcmContext.predScale =
                regionInfo.adpcmContext[ch].loopPredScale;
            m_AdpcmContextForStartOffset[ch].audioAdpcmContext.history[0] =
                static_cast<s16>(regionInfo.adpcmContext[ch].loopYn1);
            m_AdpcmContextForStartOffset[ch].audioAdpcmContext.history[1] =
                static_cast<s16>(regionInfo.adpcmContext[ch].loopYn2);
        }
        m_AdpcmContextForStartOffsetFrame = regionInfo.start;
    }
}
#endif

#if NN_WARE_VER >= NN_MAKE_VER(3, 0, 0)
void RegionManager::SetRegionInfo(const StreamSoundFile::RegionInfo* pRegionInfo,
                                  const StreamDataInfoDetail* pStreamDataInfo) {
    if (pRegionInfo == nullptr) {
        m_CurrentRegion.begin = 0;
        m_CurrentRegion.end = static_cast<position_t>(pStreamDataInfo->sampleCount);
        m_CurrentRegion.isEnabled = true;
        return;
    }

    m_CurrentRegion.begin = pRegionInfo->start;
    m_CurrentRegion.end = pRegionInfo->end;

    m_CurrentRegion.isEnabled = m_IsRegionIndexCheckEnabled ? pRegionInfo->isEnabled : true;

    if (pStreamDataInfo->sampleFormat == SampleFormat_DspAdpcm) {
        for (int ch{0}; ch < pStreamDataInfo->channelCount; ++ch) {
            m_AdpcmContextForStartOffset[ch].audioAdpcmContext.predScale =
                pRegionInfo->adpcmContext[ch].loopPredScale;
            m_AdpcmContextForStartOffset[ch].audioAdpcmContext.history[0] =
                static_cast<s16>(pRegionInfo->adpcmContext[ch].loopYn1);
            m_AdpcmContextForStartOffset[ch].audioAdpcmContext.history[1] =
                static_cast<s16>(pRegionInfo->adpcmContext[ch].loopYn2);
        }
        m_AdpcmContextForStartOffsetFrame = pRegionInfo->start;
    }
}
#endif

bool RegionManager::TryMoveNextRegion(IRegionInfoReadable* pRegionReader,
                                      StreamDataInfoDetail* pStreamDataInfo) {
    if (pStreamDataInfo->loopFlag) {
        m_CurrentRegion.begin = pStreamDataInfo->loopStart;
        m_CurrentRegion.end = static_cast<position_t>(pStreamDataInfo->sampleCount);
        m_CurrentRegion.current = m_CurrentRegion.begin;
        return true;
    }

    if (IsPreparedForRegionJump() &&
        ChangeRegion(m_CurrentRegionNo, pRegionReader, pStreamDataInfo)) {
        m_CurrentRegion.current = m_CurrentRegion.begin;
        return true;
    }

    m_CurrentRegion.current = m_CurrentRegion.end;
    return false;
}

void RegionManager::SetPosition(position_t position) {
    m_CurrentRegion.current = position;
}

void RegionManager::AddPosition(position_t position) {
    m_CurrentRegion.current += position;
}

bool RegionManager::IsInFirstRegion() const {
    return m_CurrentRegionNo == 0;
}

}  // namespace nn::atk::detail
