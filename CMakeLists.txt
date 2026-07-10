cmake_minimum_required(VERSION 3.15)

project(NintendoSDK CXX ASM)

set(NN_GFX_TARGET "NVN" CACHE STRING "Set nn::gfx build target")
set_property(CACHE NN_GFX_TARGET PROPERTY STRINGS "" "NVN")

set(NVN_API "C" CACHE STRING "Set NVN platform type")
set_property(CACHE NVN_API PROPERTY STRINGS "C" "CPP")

set(NN_SOURCES "")

if (NN_GFX_TARGET)
    # gfx_NvnHelper.cpp includes the C impl
    if (NVN_API STREQUAL "C")
        unset(NVN_API CACHE)
    endif ()

    set(NN_GFX_SOURCES
            include/nn/gfx/detail/gfx_DataContainer.h
            include/nn/gfx/detail/gfx_Fwd.h
            include/nn/gfx/detail/gfx_Misc.h
            include/nn/gfx/detail/gfx_RequiredMemory.h
            include/nn/gfx/detail/gfx_ResShaderImpl.h
            include/nn/gfx/detail/gfx_RootSignature-api.common.h
            include/nn/gfx/util/gfx_PrimitiveShape.h

            include/nn/gfx/gfx_Buffer.h
            include/nn/gfx/gfx_BufferInfo.h
            include/nn/gfx/gfx_BufferInfoData.h
            include/nn/gfx/gfx_CommandBuffer.h
            include/nn/gfx/gfx_CommandBufferInfo.h
            include/nn/gfx/gfx_CommandBufferInfoData.h
            include/nn/gfx/gfx_Common.h
            include/nn/gfx/gfx_DescriptorPool.h
            include/nn/gfx/gfx_DescriptorPoolInfo.h
            include/nn/gfx/gfx_DescriptorPoolInfoData.h
            include/nn/gfx/gfx_DescriptorSlot.h
            include/nn/gfx/gfx_Device.h
            include/nn/gfx/gfx_DeviceInfo.h
            include/nn/gfx/gfx_DeviceInfoData.h
            include/nn/gfx/gfx_Enum.h
            include/nn/gfx/gfx_GpuAddress.h
            include/nn/gfx/gfx_MemoryPool.h
            include/nn/gfx/gfx_MemoryPoolInfo.h
            include/nn/gfx/gfx_MemoryPoolInfoData.h
            include/nn/gfx/gfx_Queue.h
            include/nn/gfx/gfx_QueueInfo.h
            include/nn/gfx/gfx_QueueInfoData.h
            include/nn/gfx/gfx_ResShader.h
            include/nn/gfx/gfx_ResShaderData.h
            include/nn/gfx/gfx_ResTexture.h
            include/nn/gfx/gfx_ResTextureData.h
            include/nn/gfx/gfx_ResUserData.h
            include/nn/gfx/gfx_ResUserDataData.h
            include/nn/gfx/gfx_RootSignatureData-api.common.h
            include/nn/gfx/gfx_RootSignatureInfo.h
            include/nn/gfx/gfx_RootSignatureInfoData.h
            include/nn/gfx/gfx_Sampler.h
            include/nn/gfx/gfx_SamplerInfo.h
            include/nn/gfx/gfx_SamplerInfoData.h
            include/nn/gfx/gfx_Shader.h
            include/nn/gfx/gfx_ShaderInfo.h
            include/nn/gfx/gfx_ShaderInfoData.h
            include/nn/gfx/gfx_State.h
            include/nn/gfx/gfx_StateInfo.h
            include/nn/gfx/gfx_StateInfoData.h
            include/nn/gfx/gfx_SwapChain.h
            include/nn/gfx/gfx_SwapChainInfo.h
            include/nn/gfx/gfx_SwapChainInfoData.h
            include/nn/gfx/gfx_Sync.h
            include/nn/gfx/gfx_SyncInfo.h
            include/nn/gfx/gfx_SyncInfoData.h
            include/nn/gfx/gfx_Texture.h
            include/nn/gfx/gfx_TextureInfo.h
            include/nn/gfx/gfx_TextureInfoData.h
            include/nn/gfx/gfx_Types.h
            include/nn/gfx/gfx_VariationBase.h

            src/NintendoSDK/gfx/detail/gfx_CommonHelper.cpp
            src/NintendoSDK/gfx/detail/gfx_CommonHelper.h
            src/NintendoSDK/gfx/util/gfx_PrimitiveShape.cpp

            src/NintendoSDK/gfx/gfx_BufferInfo.cpp
            src/NintendoSDK/gfx/gfx_CommandBufferInfo.cpp
            src/NintendoSDK/gfx/gfx_DescriptorPoolInfo.cpp
            src/NintendoSDK/gfx/gfx_DeviceInfo.cpp
            src/NintendoSDK/gfx/gfx_MemoryPoolInfo.cpp
            src/NintendoSDK/gfx/gfx_QueueInfo.cpp
            src/NintendoSDK/gfx/gfx_ResShader.cpp
            src/NintendoSDK/gfx/gfx_ResTexture.cpp
            src/NintendoSDK/gfx/gfx_SamplerInfo.cpp
            src/NintendoSDK/gfx/gfx_ShaderInfo.cpp
            src/NintendoSDK/gfx/gfx_StateInfo.cpp
            src/NintendoSDK/gfx/gfx_SwapChainInfo.cpp
            src/NintendoSDK/gfx/gfx_SyncInfo.cpp
            src/NintendoSDK/gfx/gfx_TextureInfo.cpp
    )

    if (NN_GFX_TARGET STREQUAL "NVN")
        list(APPEND NN_GFX_SOURCES
                include/nn/gfx/detail/gfx_Buffer-api.nvn.8.h
                include/nn/gfx/detail/gfx_CommandBuffer-api.nvn.8.h
                include/nn/gfx/detail/gfx_Common-api.nvn.h
                include/nn/gfx/detail/gfx_DescriptorPool-api.nvn.8.h
                include/nn/gfx/detail/gfx_Device-api.nvn.8.h
                include/nn/gfx/detail/gfx_MemoryPool-api.nvn.8.h
                include/nn/gfx/detail/gfx_Pipeline-api.nvn.8.h
                include/nn/gfx/detail/gfx_Queue-api.nvn.8.h
                include/nn/gfx/detail/gfx_RootSignature-api.nvn.8.h
                include/nn/gfx/detail/gfx_Sampler-api.nvn.8.h
                include/nn/gfx/detail/gfx_Shader-api.nvn.8.h
                include/nn/gfx/detail/gfx_State-api.nvn.8.h
                include/nn/gfx/detail/gfx_SwapChain-api.nvn.8.h
                include/nn/gfx/detail/gfx_Sync-api.nvn.8.h
                include/nn/gfx/detail/gfx_Texture-api.nvn.8.h

                include/nn/gfx/gfx_BufferData-api.nvn.8.h
                include/nn/gfx/gfx_CommandBufferData-api.nvn.8.h
                include/nn/gfx/gfx_DescriptorPoolData-api.nvn.8.h
                include/nn/gfx/gfx_DeviceData-api.nvn.8.h
                include/nn/gfx/gfx_Interoperation-api.nvn.8.h
                include/nn/gfx/gfx_MemoryPoolData-api.nvn.8.h
                include/nn/gfx/gfx_PipelineData-api.nvn.8.h
                include/nn/gfx/gfx_QueueData-api.nvn.8.h
                include/nn/gfx/gfx_ResShaderData-api.nvn.h
                include/nn/gfx/gfx_SamplerData-api.nvn.8.h
                include/nn/gfx/gfx_ShaderData-api.nvn.8.h
                include/nn/gfx/gfx_StateData-api.nvn.8.h
                include/nn/gfx/gfx_SwapChainData-api.nvn.8.h
                include/nn/gfx/gfx_SyncData-api.nvn.8.h
                include/nn/gfx/gfx_TextureData-api.nvn.8.h

                src/NintendoSDK/gfx/detail/gfx_Buffer-api.nvn.8.cpp
                src/NintendoSDK/gfx/detail/gfx_CommandBuffer-api.nvn.8.cpp
                src/NintendoSDK/gfx/detail/gfx_DescriptorPool-api.nvn.8.cpp
                src/NintendoSDK/gfx/detail/gfx_Device-api.nvn.8-os.horizon.cpp
                src/NintendoSDK/gfx/detail/gfx_GlslcFunction.cpp
                src/NintendoSDK/gfx/detail/gfx_MemoryPool-api.nvn.8.cpp
                src/NintendoSDK/gfx/detail/gfx_NvnHelper-os.horizon.cpp
                src/NintendoSDK/gfx/detail/gfx_NvnHelper.cpp
                src/NintendoSDK/gfx/detail/gfx_NvnHelper.h
                src/NintendoSDK/gfx/detail/gfx_ResShader-api.nvn.8.cpp
                src/NintendoSDK/gfx/detail/gfx_Sampler-api.nvn.8.cpp
                src/NintendoSDK/gfx/detail/gfx_Shader-api.nvn.8.cpp
                src/NintendoSDK/gfx/detail/gfx_State-api.nvn.8.cpp
                src/NintendoSDK/gfx/detail/gfx_Texture-api.nvn.8.cpp

                src/NintendoSDK/gfx/util/gfx_ObjectDebugLabel-api.nvn.8.cpp
                src/NintendoSDK/gfx/gfx_Interoperation-api.nvn.cpp
        )
    endif ()

    list(APPEND NN_SOURCES ${NN_GFX_SOURCES})
endif ()

add_library(NintendoSDK OBJECT
        ${NN_SOURCES}

        include/nn/account.h
        include/nn/am.h
        include/nn/aoc.h
        include/nn/applet.h
        include/nn/atk.h
        include/nn/atk/AuxBus.h
        include/nn/atk/detail/AdvancedWaveSoundRuntime.h
        include/nn/atk/detail/BasicSound.h
        include/nn/atk/detail/SequenceSoundRuntime.h
        include/nn/atk/detail/SoundArchiveManager.h
        include/nn/atk/detail/StreamSoundRuntime.h
        include/nn/atk/detail/WaveSoundRuntime.h
        include/nn/atk/SoundArchivePlayer.h
        include/nn/atk/SoundDataManager.h
        include/nn/atk/SoundPlayer.h
        include/nn/audio.h
        include/nn/bcat.h
        include/nn/crypto.h
        include/nn/diag.h
        include/nn/err.h
        include/nn/font/font_Util.h
        include/nn/friends.h
        include/nn/fs.h
        include/nn/fs/fs_bcat.h
        include/nn/fs/fs_directories.h
        include/nn/fs/fs_files.h
        include/nn/fs/fs_mount.h
        include/nn/fs/fs_rom.h
        include/nn/fs/fs_save.h
        include/nn/fs/fs_types.h
        include/nn/g3d/BindFuncTable.h
        include/nn/g3d/ModelObj.h
        include/nn/g3d/ResFile.h
        include/nn/g3d/ResFogAnim.h
        include/nn/g3d/ResLightAnim.h
        include/nn/g3d/ResMaterial.h
        include/nn/g3d/ResMaterialAnim.h
        include/nn/g3d/ResModel.h
        include/nn/g3d/ResSceneAnim.h
        include/nn/g3d/ResShapeAnim.h
        include/nn/g3d/ResSkeletalAnim.h
        include/nn/g3d/ResSkeleton.h
        include/nn/g3d/SkeletonObj.h
        include/nn/g3d/World.h
        include/nn/hid.h
        include/nn/image.h
        include/nn/init.h
        include/nn/irs.h
        include/nn/irs/ClusteringData.h
        include/nn/irs/ClusteringProcessorConfig.h
        include/nn/irs/ClusteringProcessorState.h
        include/nn/irs/DpdProcessorConfig.h
        include/nn/irs/DpdProcessorPointingState.h
        include/nn/irs/DpdProcessorState.h
        include/nn/irs/HandAnalysisConfig.h
        include/nn/irs/HandAnalysisMode.h
        include/nn/irs/ImageTransferProcessorConfig.h
        include/nn/irs/ImageTransferProcessorFormat.h
        include/nn/irs/ImageTransferProcessorState.h
        include/nn/irs/MomentProcessorConfig.h
        include/nn/irs/MomentProcessorState.h
        include/nn/irs/MomentStatistic.h
        include/nn/irs/Rect.h
        include/nn/irs/sys.h
        include/nn/irs/TeraPluginProcessorConfig.h
        include/nn/irs/TeraPluginProcessorState.h
        include/nn/mem.h
        include/nn/nfp.h
        include/nn/nfp/nfp.h
        include/nn/nfp/nfp_types.h
        include/nn/nifm.h
        include/nn/nn.h
        include/nn/nn_BitTypes.h
        include/nn/nnmusl.hpp
        include/nn/oe.h
        include/nn/os.h
        include/nn/os/detail/os_InternalConditionVariable.h
        include/nn/os/detail/os_InternalConditionVariable-os.horizon.h
        include/nn/os/detail/os_InternalCriticalSection.h
        include/nn/os/detail/os_InternalCriticalSection-os.horizon.h
        include/nn/os/detail/os_ThreadTypes-os.horizon.h
        include/nn/os/os_ConditionVariableCommon.h
        include/nn/os/os_MessageQueueTypes.h
        include/nn/os/os_Mutex.h
        include/nn/os/os_MutexTypes.h
        include/nn/os/os_ThreadCommon.h
        include/nn/os/os_ThreadTypes.h
        include/nn/pl.h
        include/nn/prepo.h
        include/nn/ro.h
        include/nn/settings.h
        include/nn/sf.h
        include/nn/sf/hipc.h
        include/nn/sf/IServiceObject.h
        include/nn/sf/ISharedObject.h
        include/nn/socket.h
        include/nn/ssl.h
        include/nn/ssl/BuiltInManager.h
        include/nn/ssl/Connection.h
        include/nn/ssl/Context.h
        include/nn/ssl/Debug.h
        include/nn/ssl/sf/ISslConnection.h
        include/nn/ssl/sf/ISslContext.h
        include/nn/ssl/sf/ISslService.h
        include/nn/ssl/ssl-c-bindings.h
        include/nn/svc.h
        include/nn/swkbd/swkbd.h
        include/nn/time.h
        include/nn/types.h
        include/nn/ui2d/detail/TexCoordArray.h
        include/nn/ui2d/Layout.h
        include/nn/ui2d/Material.h
        include/nn/ui2d/Pane.h
        include/nn/ui2d/Parts.h
        include/nn/ui2d/Types.h
        include/nn/ui2d/Util.h
        include/nn/util.h
        include/nn/util/detail/util_ArithmeticImpl.h
        include/nn/util/detail/util_TypeTraits.h
        include/nn/util/AccessorBase.h
        include/nn/util/MathTypes.h
        include/nn/util/util_Arithmetic.h
        include/nn/util/util_BinaryFormat.h
        include/nn/util/util_BinTypes.h
        include/nn/util/util_BitArray.h
        include/nn/util/util_BitFlagSet.h
        include/nn/util/util_BitPack.h
        include/nn/util/util_BitUtil.h
        include/nn/util/util_BytePtr.h
        include/nn/util/util_IntrusiveList.h
        include/nn/util/util_ResDic.h
        include/nn/util/util_StringView.h
        include/nn/util/util_TypedStorage.h
        include/nn/vfx/Config.h
        include/nn/vfx/Heap.h
        include/nn/vfx/Resource.h
        include/nn/vfx/System.h
        include/nn/vi.h
        include/nn/xcd.h

        include/nvn/nvn.h
        include/nvn/nvn_Cpp.h
        include/nvn/nvn_CppFuncPtrBase.h
        include/nvn/nvn_CppMethods.h
        include/nvn/nvn_DeviceConstantsNX.h
        include/nvn/nvn_FuncPtrBase.h
        include/nvn/nvn_FuncPtrInline.h

        include/nvnTool/nvnTool_GlslcInterface.h

        include/vapours/results/cal_results.hpp
        include/vapours/results/capsrv_results.hpp
        include/vapours/results/creport_results.hpp
        include/vapours/results/debug_results.hpp
        include/vapours/results/dmnt_results.hpp
        include/vapours/results/erpt_results.hpp
        include/vapours/results/err_results.hpp
        include/vapours/results/exosphere_results.hpp
        include/vapours/results/fatal_results.hpp
        include/vapours/results/fs_results.hpp
        include/vapours/results/hipc_results.hpp
        include/vapours/results/i2c_results.hpp
        include/vapours/results/kvdb_results.hpp
        include/vapours/results/loader_results.hpp
        include/vapours/results/lr_results.hpp
        include/vapours/results/ncm_results.hpp
        include/vapours/results/nfp_results.hpp
        include/vapours/results/nim_results.hpp
        include/vapours/results/ns_results.hpp
        include/vapours/results/os_results.hpp
        include/vapours/results/pgl_results.hpp
        include/vapours/results/pm_results.hpp
        include/vapours/results/psc_results.hpp
        include/vapours/results/results_common.hpp
        include/vapours/results/ro_results.hpp
        include/vapours/results/settings_results.hpp
        include/vapours/results/sf_results.hpp
        include/vapours/results/sm_results.hpp
        include/vapours/results/spl_results.hpp
        include/vapours/results/svc_results.hpp
        include/vapours/results/time_results.hpp
        include/vapours/results/updater_results.hpp
        include/vapours/results/vi_results.hpp
        include/vapours/results.hpp

        include/nv.h

        src/NintendoSDK/nvn/nvn_CppFuncPtrImpl.h
        src/NintendoSDK/nvn/nvn_FuncPtrImpl.h
        src/NintendoSDK/nvn/nvn_Impl.cpp
)

target_include_directories(NintendoSDK PUBLIC include/)
target_include_directories(NintendoSDK PRIVATE src/NintendoSDK/)
target_compile_options(NintendoSDK PRIVATE -fno-strict-aliasing)
target_compile_options(NintendoSDK PRIVATE -fno-exceptions)
target_compile_options(NintendoSDK PRIVATE -Wall -Wextra)
target_compile_options(NintendoSDK PRIVATE -Wno-invalid-offsetof)

if (NVN_API)
    target_compile_definitions(NintendoSDK PRIVATE NVN_API_${NVN_API})
endif ()

function(nn_ver ID)
    set(VER ${${ID}})
    string(REPLACE "." ";" VER "${VER}")

    list(LENGTH VER SEGS)
    if (SEGS LESS 3)
        set(DEF_VER 1 0 0)
        list(SUBLIST DEF_VER ${SEGS} -1 DEF_VER)
        list(APPEND VER ${DEF_VER})

        message(AUTHOR_WARNING "${ID} semver is not complete (%d.%d.%d): ${${ID}}")
    endif ()

    list(POP_FRONT VER MAJOR MINOR PATCH)

    foreach (VAR IN ITEMS MAJOR MINOR PATCH)
        target_compile_definitions(NintendoSDK PRIVATE ${ID}_${VAR}=${${VAR}})
    endforeach ()
endfunction()

nn_ver(NN_WARE)
nn_ver(NN_SDK)
target_compile_definitions(NintendoSDK PRIVATE NN_SDK_TYPE="${NN_SDK_TYPE}")

