#pragma once

#include "nn/atk/atk_SoundDataManager.h"
#include "nn/atk/atk_SoundPlayer.h"
#include "nn/atk/detail/atk_BasicSoundPlayer.h"
#include "nn/atk/detail/atk_DisposeCallback.h"
#include "nn/atk/detail/atk_LoaderManager.h"
#include "nn/atk/detail/atk_NoteOnCallback.h"
#include "nn/atk/detail/atk_PlayerHeap.h"
#include "nn/atk/detail/atk_PlayerHeapDataManager.h"
#include "nn/atk/detail/atk_SoundArchiveLoader.h"
#include "nn/atk/detail/thread/atk_SoundThread.h"
#include "nn/atk/detail/thread/atk_Task.h"

namespace nn::atk {
struct SequenceUserProcCallbackParam {
    s16* localVariable;
    s16* globalVariable;
    s16* trackVariable;
    bool cmpFlag;
};
static_assert(sizeof(SequenceUserProcCallbackParam) == 0x20);

using SequenceUserProcCallback = void(*)(u16,SequenceUserProcCallbackParam*,void*);

namespace detail::driver {
class SequenceSoundLoader {
public:
    struct LoadInfo {
        LoadInfo(SoundArchive* arc, SoundDataManager* mgr, 
                 LoadItemInfo* seq, LoadItemInfo* banks, SoundPlayer* player);

        SoundArchive* soundArchive;
        SoundDataManager* soundDataManager;
        LoadItemInfo* loadInfoSeq;
        LoadItemInfo* loadInfoBanks[4];
        SoundPlayer* soundPlayer;
    };
    static_assert(sizeof(LoadInfo) == 0x40);

    struct Arg {
        SoundArchive* soundArchive;
        SoundDataManager* soundDataManager;
        SoundPlayer* soundPlayer;
        LoadItemInfo loadInfoSeq;
        LoadItemInfo loadInfoBanks[4];
    };
    static_assert(sizeof(Arg) == 0x68);

    struct Data {
        void* seqFile;
        void* bankFiles[4];
        void* warcFiles[4];
        bool warcIsIndividuals[4];
    };
    static_assert(sizeof(Data) == 0x50);

    class DataLoadTask : Task {
    public:
        DataLoadTask();
        ~DataLoadTask() override;

        bool TryAllocPlayerHeap();

        void Execute(TaskProfileLogger& logger) override;

    private:
        Arg m_Arg;
        Data m_Data;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;
        bool m_IsLoadSuccess;
        u8 m_Padding[3];
    };
    static_assert(sizeof(DataLoadTask) == 0x118);

    class FreePlayerHeapTask : Task {
    public:
        FreePlayerHeapTask();
        ~FreePlayerHeapTask() override;

        void Initialize();
        void Execute(TaskProfileLogger& logger) override;

    private:
        Arg m_Arg;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;

    };

    ~SequenceSoundLoader();

    void Initialize(const Arg& arg);

    void Finalize();

    bool IsInUse();

    bool TryWait();

private:
    friend LoaderManager<SequenceSoundLoader>;

    DataLoadTask m_Task;
    FreePlayerHeapTask m_FreePlayerHeapTask;
    PlayerHeapDataManager m_PlayerHeapDataManager;
    util::IntrusiveListNode m_LinkForLoaderManager;
};
static_assert(sizeof(SequenceSoundLoader) == 0x4b0);

using SequenceSoundLoaderManager = LoaderManager<SequenceSoundLoader>;

} // namespace nn::atk::detail::driver
} // namespace nn::atk