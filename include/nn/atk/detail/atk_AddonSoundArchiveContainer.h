#pragma once

#include <nn/os.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundDataManager.h>

namespace nn::atk {
class AddonSoundArchive;

namespace detail {
class AddonSoundArchiveContainer {
public:
    constexpr static s32 SoundArchiveNameCountMax = 64;

    AddonSoundArchiveContainer();
    ~AddonSoundArchiveContainer();

    bool Initialize(const char* name, 
                    const AddonSoundArchive* pAddonSoundArchive, 
                    const SoundDataManager* pSoundDataManager);
    
    void Finalize();
    
    bool IsSameName(const char* name) const;

    void SetAddTick(const os::Tick& addTick);

    util::IntrusiveListNode m_ElementLink;

private:
    bool m_IsActive;
    u8 m_Padding[3];
    SoundArchive* m_pSoundArchive;
    SoundDataManager* m_pSoundDataManager;
    char m_SoundArchiveName[SoundArchiveNameCountMax];
    os::Tick m_AddTick;
};
static_assert(sizeof(AddonSoundArchiveContainer) == 0x70);
} // namespace nn::atk::detail
} // namespace nn::atk