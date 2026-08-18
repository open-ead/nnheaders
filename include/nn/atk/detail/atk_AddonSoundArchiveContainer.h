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
    AddonSoundArchiveContainer();
    ~AddonSoundArchiveContainer();

    bool Initialize(const char* soundArchiveName, 
                    const AddonSoundArchive* pSoundArchive, 
                    const SoundDataManager* pSoundDataManager);
    
    void Finalize();
    
    bool IsSameName(const char* name) const;

    bool IsActive() const {
        return m_IsActive;
    }

    const SoundArchive* GetSoundArchive() const {
        return m_pSoundArchive;
    }

    const SoundDataManager* GetSoundDataManager() const {
        return m_pSoundDataManager;
    }

    const char* GetSoundArchiveName() const {
        return m_SoundArchiveName;
    }

    void SetAddTick(const os::Tick& tick);

    os::Tick GetAddTick() const { return m_AddTick; }

    static const int SoundArchiveNameLengthMax = 64;
    util::IntrusiveListNode m_ElementLink;

private:
    bool m_IsActive {false};
    u8 m_Padding[3];
    const SoundArchive* m_pSoundArchive {};
    const SoundDataManager* m_pSoundDataManager {};
    char m_SoundArchiveName[SoundArchiveNameLengthMax];
    os::Tick m_AddTick {0};
};
static_assert(sizeof(AddonSoundArchiveContainer) == 0x70);
} // namespace nn::atk::detail
} // namespace nn::atk