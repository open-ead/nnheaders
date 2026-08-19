#pragma once

#include <nn/atk/atk_SoundArchive.h>

namespace nn::atk::detail {
class SoundArchiveParametersHook {
public:
    bool GetIsEnable() const { return m_IsEnable; }
    void SetIsEnable(bool value) { m_IsEnable = value; }

    const char* GetItemLabel(SoundArchive::ItemId id) const {
        if (m_IsEnable)
            return GetItemLabelImpl(id);

        return nullptr;
    }

    SoundArchive::ItemId GetItemId(const char* itemLabel) const {
        if (m_IsEnable)
            return GetItemIdImpl(itemLabel);

        return SoundArchive::InvalidId;
    }

    SoundArchive::SoundType GetSoundType(const char* itemLabel) {
        if (m_IsEnable)
            return GetSoundTypeImpl(itemLabel);

        return SoundArchive::SoundType_Invalid;
    }

    bool ReadSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SoundInfo* info) const {
        if (m_IsEnable)
            return ReadSoundInfoImpl(soundId, info);

        return false;
    }

    bool ReadBankInfo(SoundArchive::ItemId bankId, SoundArchive::BankInfo* info) const {
        if (m_IsEnable)
            return ReadBankInfoImpl(bankId, info);

        return false;
    }

    bool ReadPlayerInfo(SoundArchive::ItemId playerId, SoundArchive::PlayerInfo* info) const {
        if (m_IsEnable)
            return ReadPlayerInfoImpl(playerId, info);

        return false;
    }

    bool ReadSoundGroupInfo(SoundArchive::ItemId soundGroupId, SoundArchive::SoundGroupInfo* info) const;
    bool ReadGroupInfo(SoundArchive::ItemId groupId, SoundArchive::GroupInfo* info) const;
    bool ReadFileInfo(SoundArchive::FileId id, SoundArchive::FileInfo* info, int index) const;
    
    bool ReadSoundArchivePlayerInfo(SoundArchive::SoundArchivePlayerInfo* info) const {
        if (m_IsEnable)
            return ReadSoundArchivePlayerInfoImpl(info);

        return false;
    }

    bool ReadSound3DInfo(SoundArchive::ItemId soundId, SoundArchive::Sound3DInfo* info) const;
    bool ReadWaveArchiveInfo(SoundArchive::ItemId warcId, SoundArchive::WaveArchiveInfo* info) const;
    
    bool ReadSequenceSoundInfo(SoundArchive::ItemId soundId, SoundArchive::SequenceSoundInfo* info) const {
        if (m_IsEnable)
            return ReadSequenceSoundInfoImpl(soundId, info);

        return false;
    }

    bool ReadStreamSoundInfo(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo* info) const {
        if (m_IsEnable)
            return ReadStreamSoundInfoImpl(soundId, info);

        return false;
    }

    bool ReadStreamSoundInfo2(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo2* info) const {
        if (m_IsEnable)
            return ReadStreamSoundInfo2Impl(soundId, info);

        return false;
    }
    
    bool ReadWaveSoundInfo(SoundArchive::ItemId soundId, SoundArchive::WaveSoundInfo* info) const;
    
    bool ReadSoundUserParam(u32* pOutValue, SoundArchive::ItemId soundId, int index) const {
        if (m_IsEnable)
            return ReadSoundUserParamImpl(pOutValue, soundId, index);

        return false;
    }

    u32 GetSoundUserParam(SoundArchive::ItemId soundId, u32 userParam) const {
        return GetSoundUserParamImpl(soundId, userParam);
    }

protected:
    virtual void Impl0();
    virtual void Impl1();
    virtual void Impl2();
    virtual const char* GetItemLabelImpl(SoundArchive::ItemId id) const;
    virtual SoundArchive::ItemId GetItemIdImpl(const char* itemLabel) const;
    virtual SoundArchive::SoundType GetSoundTypeImpl(const char* itemLabel) const;
    virtual bool ReadSoundInfoImpl(SoundArchive::ItemId soundId, SoundArchive::SoundInfo* info) const;
    virtual bool ReadBankInfoImpl(SoundArchive::ItemId bankId, SoundArchive::BankInfo* info) const;
    virtual bool ReadPlayerInfoImpl(SoundArchive::ItemId playerId, SoundArchive::PlayerInfo* info) const;
    virtual void Impl9();
    virtual void Impl10();
    virtual void Impl11();
    virtual void Impl12();
    virtual bool ReadSoundArchivePlayerInfoImpl(SoundArchive::SoundArchivePlayerInfo* info) const;
    virtual void Impl14();
    virtual bool ReadSequenceSoundInfoImpl(SoundArchive::ItemId soundId, SoundArchive::SequenceSoundInfo* info) const;
    virtual bool ReadStreamSoundInfoImpl(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo* info) const;
    virtual bool ReadStreamSoundInfo2Impl(SoundArchive::ItemId soundId, SoundArchive::StreamSoundInfo2* info) const;
    virtual void Impl18();
    virtual bool ReadSoundUserParamImpl(u32* pOutValue, SoundArchive::ItemId soundId, int index) const;
    virtual u32 GetSoundUserParamImpl(SoundArchive::ItemId soundId, u32 userParam) const;

private:
    bool m_IsEnable;
};
} // namespace nn::atk::detail