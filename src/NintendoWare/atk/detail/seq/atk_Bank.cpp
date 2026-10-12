#include <nn/atk/atk_Bank.h>

#include <algorithm>

#include <nn/atk/atk_WaveFileReader.h>

namespace {

const float VelocityMaxR{1.0f / 127.0f};

const float InstrumentVolumeCriterionR{1.0f / 127.0f};

}  // namespace

namespace nn::atk::detail::driver {

Bank::Bank() = default;

Bank::~Bank() = default;

Channel* Bank::NoteOn(const BankFileReader& bankReader, const WaveArchiveFileReader& warcReader,
                      const NoteOnInfo& noteOnInfo) const {
    VelocityRegionInfo regionInfo;

    if (!bankReader.ReadVelocityRegionInfo(&regionInfo, noteOnInfo.prgNo, noteOnInfo.key,
                                           noteOnInfo.velocity))
        return nullptr;

    const void* waveFile{warcReader.GetWaveFile(regionInfo.waveIndex)};

    WaveInfo waveInfo;

    {
        WaveFileReader reader{waveFile, WaveType_Nwwav};

        if (!reader.ReadWaveInfo(&waveInfo, nullptr))
            return nullptr;
    }

    Channel* pChannel{Channel::AllocChannel(std::min(waveInfo.channelCount, 2), noteOnInfo.priority,
                                            noteOnInfo.channelCallback,
                                            noteOnInfo.channelCallbackData)};

    if (pChannel == nullptr)
        return nullptr;

    pChannel->SetKey(noteOnInfo.key, regionInfo.originalKey);
    pChannel->SetVelocity(CalcChannelVelocityVolume(noteOnInfo.velocity));
    pChannel->SetInstrumentVolume(static_cast<float>(regionInfo.volume) *
                                  InstrumentVolumeCriterionR);
    pChannel->SetTune(regionInfo.pitch);
    pChannel->SetAttack(regionInfo.adshrCurve.GetAttack());
    pChannel->SetHold(regionInfo.adshrCurve.GetHold());
    pChannel->SetDecay(regionInfo.adshrCurve.GetDecay());
    pChannel->SetSustain(regionInfo.adshrCurve.GetSustain());
    pChannel->SetRelease(regionInfo.adshrCurve.GetRelease());

    float initPan{static_cast<float>(noteOnInfo.initPan + regionInfo.pan - 64) / 63.0f};
    pChannel->SetInitPan(initPan);

    pChannel->SetKeyGroupId(regionInfo.keyGroup);
    pChannel->SetIsIgnoreNoteOff(regionInfo.isIgnoreNoteOff);
    pChannel->SetInterpolationType(regionInfo.interpolationType);
    pChannel->SetUpdateType(noteOnInfo.updateType);
#if NN_SDK_VER < NN_MAKE_VER(4, 0, 0)
    pChannel->SetSubMixIndex(noteOnInfo.subMixIndex);
    pChannel->Start(waveInfo, noteOnInfo.length, 0);
#else
    pChannel->SetOutputReceiver(noteOnInfo.pOutputReceiver);
    pChannel->Start(waveInfo, noteOnInfo.length, 0, false);
#endif

    return pChannel;
}

float Bank::CalcChannelVelocityVolume(u8 velocity) {
    return static_cast<float>(velocity) * VelocityMaxR;
}

}  // namespace nn::atk::detail::driver
