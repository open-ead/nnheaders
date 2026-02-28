#pragma once

#include <nn/atk/atk_SoundDataManager.h>
#include <nn/atk/detail/atk_LoaderManager.h>
#include <nn/atk/detail/atk_PlayerHeap.h>
#include <nn/atk/detail/atk_PlayerHeapDataManager.h>
#include <nn/atk/detail/thread/atk_Task.h>

namespace nn::atk::detail::driver {
class WaveSoundLoader;
using WaveSoundLoaderManager = LoaderManager<WaveSoundLoader>;

class WaveSoundLoader {
public:
    struct Arg {
        SoundArchive* soundArchive;
        SoundDataManager* soundDataManager;
        SoundPlayer* soundPlayer;
        LoadItemInfo loadInfoWsd;
        s32 index;
    };
    static_assert(sizeof(Arg) == 0x30);

    struct Data {
        void* wsdFile;
        void* waveFile;
    };
    static_assert(sizeof(Data) == 0x10);

    struct LoadInfo {
        SoundArchive* soundArchive;
        SoundDataManager* soundDataManager;
        LoadItemInfo* loadInfoWsd;
        SoundPlayer* soundPlayer;
    };
    static_assert(sizeof(LoadInfo) == 0x20);

    class DataLoadTask : Task {
    public:
        ~DataLoadTask() override;
        
        void Initialize();

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
    static_assert(sizeof(DataLoadTask) == 0xa0);

    class FreePlayerHeapTask : Task {
    public:
        ~FreePlayerHeapTask() override;

        void Initialize();

        void Execute(TaskProfileLogger& logger) override;

    private:
        Arg m_Arg;
        PlayerHeap* m_pPlayerHeap;
        PlayerHeapDataManager* m_pPlayerHeapDataManager;
    };

    WaveSoundLoader();
    ~WaveSoundLoader();

    void Initialize(const Arg& arg);
    void Finalize();

    bool TryWait();

    bool IsInUse();

private:
    friend WaveSoundLoaderManager;

    DataLoadTask m_Task;
    FreePlayerHeapTask m_FreePlayerHeapTask;
    PlayerHeapDataManager m_PlayerHeapDataManager;
    util::IntrusiveListNode m_LinkForLoaderManager;
};
} // namespace nn::atk::detail::driver