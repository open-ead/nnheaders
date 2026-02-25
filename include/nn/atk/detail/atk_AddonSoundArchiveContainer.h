#pragma once

#include <nn/os.h>
#include <nn/types.h>
#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/atk_SoundArchive.h>
#include <nn/atk/atk_SoundDataManager.h>

namespace nn::atk {
class AddonSoundArchive;

namespace detail {
class AddonSoundArchiveContainer {
public:
    AddonSoundArchiveContainer();
    ~AddonSoundArchiveContainer();

    bool Initialize(const char* name, 
                    const AddonSoundArchive* pAddonSoundArchive, 
                    const SoundDataManager* pSoundDataManager);
    
    void Finalize();
    
    bool IsSameName(const char* name) const;

    void SetAddTick(const os::Tick& addTick);

private:
    util::IntrusiveListNode m_ElementLink;
    bool m_IsActive;
    u8 m_Padding[3];
    SoundArchive* m_pSoundArchive;
    SoundDataManager* m_pSoundDataManager;
    char m_SoundArchiveName[64];
    os::Tick m_AddTick;
};
static_assert(sizeof(AddonSoundArchiveContainer) == 0x70);
} // namespace nn::atk::detail
} // namespace nn::atk