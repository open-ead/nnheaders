#include <nn/g3d/ResFile.h>

#include <nn/gfx/gfx_MemoryPool.h>

#include <nn/g3d/ResMaterialAnim.h>
#include <nn/g3d/ResModel.h>
#include <nn/g3d/ResSceneAnim.h>
#include <nn/g3d/ResShapeAnim.h>
#include <nn/g3d/ResSkeletalAnim.h>

#include <nn/g3d/detail/g3d_CommonHelper.h>

namespace nn::g3d {

bool ResFile::IsValid(const void* modelSrc) {
    return reinterpret_cast<const util::BinaryFileHeader*>(modelSrc)->IsValid(Signature, 8, 0, 0);
}

void ResFile::Relocate() {
    if (!fileHeader.IsRelocated())
        fileHeader.GetRelocationTable()->Relocate();
}

void ResFile::Unrelocate() {
    if (fileHeader.IsRelocated())
        fileHeader.GetRelocationTable()->Unrelocate();
}

ResFile* ResFile::ResCast(void* ptr) {
    auto* file = reinterpret_cast<ResFile*>(ptr);

    if (ptr != nullptr) {
        file->Relocate();
        file->fileHeader.IsEndianReverse();
    }

    return file;
}

s32 ResFile::BindTexture(TextureBindCallback callback, void* ptr) {
    s32 result = 0;

    s32 mdlCount = modelCount;
    for (s32 i{0}; i < mdlCount; ++i)
        result |= pModels.Get()[i].BindTexture(callback, ptr);

    s32 matAnimCount = materialAnimCount;
    for (s32 i{0}; i < matAnimCount; ++i)
        result |= pMaterialAnims.Get()[i].BindTexture(callback, ptr);

    return result;
}

void ResFile::ReleaseTexture() {
    s32 mdlCount = modelCount;
    for (s32 i{0}; i < mdlCount; ++i)
        pModels.Get()[i].ReleaseTexture();

    s32 matAnimCount = materialAnimCount;
    for (s32 i{0}; i < matAnimCount; ++i)
        pMaterialAnims.Get()[i].ReleaseTexture();
}

void ResFile::Setup(gfx::Device* device) {
    detail::UseMiddleWare();

    if (pMemoryPool.Get() != nullptr && pMemoryPoolInfo.Get() != nullptr)
        pMemoryPool.Get()->Initialize(device, *pMemoryPoolInfo.Get(), "g3d");

    s32 mdlCount = modelCount;
    for (s32 i{0}; i < mdlCount; ++i)
        pModels.Get()[i].Setup(device);
}

void ResFile::Setup(gfx::Device* device, gfx::MemoryPool* memPool, s64 bufferOffset,
                    [[maybe_unused]] u64 p3) {
    if (pMemoryPool.Get() != nullptr && pMemoryPoolInfo.Get() != nullptr)
        bufferOffset = reinterpret_cast<ptrdiff_t>(pMemoryPoolInfo.Get()->GetPoolMemory()) +
                       (bufferOffset - reinterpret_cast<ptrdiff_t>(this));

    s32 mdlCount = modelCount;
    for (s32 i{0}; i < mdlCount; ++i)
        pModels.Get()[i].Setup(device, memPool, bufferOffset);
}

void ResFile::Cleanup(gfx::Device* device) {
    s32 mdlCount = modelCount;
    for (s32 i{0}; i < mdlCount; ++i)
        pModels.Get()[i].Cleanup(device);

    if (pMemoryPoolInfo.Get() != nullptr &&
        pMemoryPool.Get()->ToData()->state != gfx::MemoryPool::DataType::State_NotInitialized)
        pMemoryPool.Get()->Finalize(device);
}

void ResFile::Reset() {
    s32 mdlCount = modelCount;
    for (s32 i{0}; i < mdlCount; ++i)
        pModels.Get()[i].Reset();

    s32 sklAnimCount = skeletalAnimCount;
    for (s32 i{0}; i < sklAnimCount; ++i)
        pSkeletalAnims.Get()[i].Reset();

    s32 matAnimCount = materialAnimCount;
    for (s32 i{0}; i < matAnimCount; ++i)
        pMaterialAnims.Get()[i].Reset();

    s32 shpAnimCount = shapeAnimCount;
    for (s32 i{0}; i < shpAnimCount; ++i)
        pShapeAnims.Get()[i].Reset();

    s32 scnAnimCount = sceneAnimCount;
    for (s32 i{0}; i < scnAnimCount; ++i)
        pSceneAnims.Get()[i].Reset();

    padding = 0;
}

}  // namespace nn::g3d