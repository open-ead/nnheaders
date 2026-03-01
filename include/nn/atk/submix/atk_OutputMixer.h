#pragma once

#include <nn/util/util_IntrusiveList.h>

#include <nn/atk/effect/atk_EffectBase.h>
#include <nn/atk/effect/atk_EffectAux.h>
#include <nn/atk/fnd/os/atkfnd_CriticalSection.h>
#include <nn/atk/submix/atk_OutputReceiver.h>

namespace nn::atk {
class OutputMixer : protected OutputReceiver {
public:
    using EffectList = util::IntrusiveList<EffectBase, 
                            util::IntrusiveListMemberNodeTraits<EffectBase, &EffectBase::m_Link>>;
    using EffectAuxList = util::IntrusiveList<EffectAux, 
                            util::IntrusiveListMemberNodeTraits<EffectAux, &EffectAux::m_AuxLinkNode>>;

    OutputMixer();

    static size_t GetRequiredMemorySize(s32 bus, bool isEffectEnabled);

    void Initialize(s32 bus, bool isEffectEnabled, void* buffer, size_t bufferSize);
    void Finalize();
    
    bool HasEffect(s32 bus) const;

    bool AppendEffect(EffectBase* pEffect, s32 bus, void* buffer, size_t bufferSize);
    bool AppendEffect(EffectAux* pEffect, s32 bus, void* buffer, size_t bufferSize);

    bool RemoveEffect(EffectBase* pEffect, s32 bus);
    bool RemoveEffect(EffectAux* pEffect, s32 bus);
    
    void ClearEffect(s32 bus);

    void UpdateEffectAux();

    void OnChangeOutputMode();

    virtual void AppendEffectImpl(EffectBase* pEffect, s32 bus, void* buffer, size_t bufferSize);
    virtual void AppendEffectImpl(EffectAux* pEffect, s32 bus, void* buffer, size_t bufferSize);

    void RemoveEffectImpl(EffectBase* pEffect, s32 bus);
    void RemoveEffectImpl(EffectAux* pEffect, s32 bus);

    void ClearEffectImpl(s32 bus);

private:
    detail::fnd::CriticalSection m_EffectListLock;
    EffectList* m_pEffectList;
    EffectAuxList* m_pEffectAuxList;
    bool m_IsEffectEnabled;
};
static_assert(sizeof(OutputMixer) == 0x40);
} // namespace nn::atk