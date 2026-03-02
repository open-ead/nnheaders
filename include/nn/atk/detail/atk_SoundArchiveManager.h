#pragma once

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/detail/atk_AddonSoundArchiveContainer.h>
#include <nn/atk/detail/atk_IntrusiveList.h>

namespace nn::atk::detail {
class SoundArchiveManager {
public:
    using ContainerList = IntrusiveList<AddonSoundArchiveContainer>;

    class SnapShot {
    public:
    private:
        SoundArchive* m_MainSoundArchive;
        SoundDataManager* m_MainSoundDataManager;
        SoundArchive* m_CurrentSoundArchive;
        SoundDataManager* m_CurrentSoundDataManager;
    };

    SoundArchiveManager();
    ~SoundArchiveManager();

    void Initialize(const SoundArchive* pSoundArchive, const SoundDataManager* pSoundDataManager);

    void ChangeTargetArchive(const char* soundArchiveName);

    void Finalize();

    void Add(AddonSoundArchiveContainer&);
    void Remove(AddonSoundArchiveContainer&);

    bool IsAvailable() const;

    AddonSoundArchive* GetAddonSoundArchive(const char*) const;
    SoundDataManager* GetAddonSoundDataManager(const char*) const;
    AddonSoundArchiveContainer* GetAddonSoundArchiveContainer(s32) const;
    AddonSoundArchiveContainer* GetAddonSoundArchiveContainer(s32);

    void SetParametersHook(SoundArchiveParametersHook*);
    SoundArchiveParametersHook* GetParametersHook() const;
    
private:
    SoundArchive* m_pMainSoundArchive;
    SoundDataManager* m_pMainSoundDataManager;
    ContainerList m_ContainerList;
    SoundArchive* m_pCurrentSoundArchive;
    SoundDataManager* m_pCurrentSoundDataManager;
    SoundArchiveParametersHook* m_pParametersHook;
};
static_assert(sizeof(SoundArchiveManager) == 0x38);
}  // namespace nn::atk::detail
