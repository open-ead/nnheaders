/**
 * @file audio.h
 * @brief Audio implementation.
 */

#pragma once

#include <nn/os.h>

#include <nn/audio/audio_Adpcm.h>
#include <nn/audio/audio_AudioRendererTypes.h>
#include <nn/audio/audio_Common.h>
#include <nn/audio/audio_EffectTypes.h>
#include <nn/audio/audio_FinalMixTypes.h>
#include <nn/audio/audio_MemoryPoolTypes.h>
#include <nn/audio/audio_PerformanceMetrics.h>
#include <nn/audio/audio_PerformanceMetricsTypes.h>
#include <nn/audio/audio_SampleFormat.h>
#include <nn/audio/audio_SinkTypes.h>
#include <nn/audio/audio_SubMixTypes.h>
#include <nn/audio/audio_VoiceTypes.h>
#include <nn/audio/audio_WaveBuffer.h>

namespace nn {
namespace audio {
// Common audio
void AcquireAudioDeviceSwitchNotification(nn::os::SystemEvent* event);
s32 ListAudioDeviceName(nn::audio::AudioDeviceName* buffer, s32 bufferCount);
Result SetAudioDeviceOutputVolume(nn::audio::AudioDeviceName const* device, float volume);
u32 GetActiveChannelCount();

// Audio Renderer base APIs
void InitializeAudioRendererParameter(nn::audio::AudioRendererParameter* inParameter);
bool IsValidAudioRendererParameter(nn::audio::AudioRendererParameter const& inParameter);
size_t GetAudioRendererWorkBufferSize(nn::audio::AudioRendererParameter const& inParameter);
size_t GetAudioRendererConfigWorkBufferSize(nn::audio::AudioRendererParameter const& inParameter);
Result InitializeAudioRendererConfig(nn::audio::AudioRendererConfig* outConfig,
                                     nn::audio::AudioRendererParameter const& inParameter,
                                     void* buffer, size_t bufferSize);
Result OpenAudioRenderer(nn::audio::AudioRendererHandle* outHandle,
                         nn::audio::AudioRendererParameter const& inParameter, void* workBuffer,
                         size_t workBufferSize);
void CloseAudioRenderer(nn::audio::AudioRendererHandle handle);
Result StartAudioRenderer(nn::audio::AudioRendererHandle handle);
Result StopAudioRenderer(nn::audio::AudioRendererHandle handle);
Result RequestUpdateAudioRenderer(nn::audio::AudioRendererHandle handle,
                                  nn::audio::AudioRendererConfig const* config);

// Audio Renderer MemoryPool APIs
bool IsMemoryPoolAttached(nn::audio::MemoryPoolType const* pool);
bool RequestAttachMemoryPool(nn::audio::MemoryPoolType* pool);
bool RequestDetachMemoryPool(nn::audio::MemoryPoolType* pool);
bool AcquireMemoryPool(nn::audio::AudioRendererConfig* config, nn::audio::MemoryPoolType* outPool,
                       void* address, size_t size);
void ReleaseMemoryPool(nn::audio::AudioRendererConfig* config, nn::audio::MemoryPoolType* pool);
void* GetMemoryPoolAddress(nn::audio::MemoryPoolType const* pool);
size_t GetMemoryPoolSize(nn::audio::MemoryPoolType const* pool);
MemoryPoolState GetMemoryPoolState(nn::audio::MemoryPoolType const* pool);

// Audio Renderer Effect APIs
void SetDelayInputOutput(nn::audio::DelayType* delay, s8 const* input, s8 const* output, s32 count);
void* RemoveDelay(nn::audio::AudioRendererConfig* config, nn::audio::DelayType* delay,
                  nn::audio::FinalMixType* mix);
void* RemoveDelay(nn::audio::AudioRendererConfig* config, nn::audio::DelayType* delay,
                  nn::audio::SubMixType* mix);
bool IsDelayRemovable(nn::audio::DelayType* delay);

size_t GetRequiredBufferSizeForAuxSendReturnBuffer(nn::audio::AudioRendererParameter const* config,
                                                   s32 mixBufferFrameCount, s32 channelCount);
Result AddAux(nn::audio::AudioRendererConfig* config, nn::audio::AuxType* aux,
              nn::audio::FinalMixType* mix, void* sendBuffer, void* returnBuffer,
              size_t bufferSize);
Result AddAux(nn::audio::AudioRendererConfig* config, nn::audio::AuxType* aux,
              nn::audio::SubMixType* mix, void* sendBuffer, void* returnBuffer, size_t bufferSize);
void RemoveAux(nn::audio::AudioRendererConfig* config, nn::audio::AuxType* aux,
               nn::audio::FinalMixType* mix);
void RemoveAux(nn::audio::AudioRendererConfig* config, nn::audio::AuxType* aux,
               nn::audio::SubMixType* mix);
void SetAuxEnabled(nn::audio::AuxType* aux, bool enable);
void SetAuxInputOutput(nn::audio::AuxType* aux, s8 const* input, s8 const* output, s32 count);
bool IsAuxRemovable(nn::audio::AuxType* aux);
s32 GetAuxSampleCount(nn::audio::AuxType const* aux);
s32 GetAuxSampleRate(nn::audio::AuxType const* aux);
s32 ReadAuxSendBuffer(nn::audio::AuxType* aux, s32* buffer, s32 count);
s32 WriteAuxReturnBuffer(nn::audio::AuxType* aux, s32 const* buffer, s32 count);

// Audio Renderer Performance APIs
size_t
GetRequiredBufferSizeForPerformanceFrames(nn::audio::AudioRendererParameter const& inParameter);
void* SetPerformanceFrameBuffer(nn::audio::AudioRendererConfig* config, void* buffer,
                                size_t bufferSize);

// Audio Renderer Sink APIs
Result AddDeviceSink(nn::audio::AudioRendererConfig* config, nn::audio::DeviceSinkType* sink,
                     nn::audio::FinalMixType* mix, s8 const* input, s32 inputCount,
                     char const* deviceName);
void RemoveDeviceSink(nn::audio::AudioRendererConfig* config, nn::audio::DeviceSinkType* sink,
                      nn::audio::FinalMixType* mix);
u32 GetSinkNodeId(nn::audio::DeviceSinkType const* sink);

Result AddCircularBufferSink(nn::audio::AudioRendererConfig* config,
                             nn::audio::CircularBufferSinkType* sink, nn::audio::FinalMixType* mix,
                             s8 const* input, s32 inputCount, void* buffer, size_t bufferSize,
                             nn::audio::SampleFormat sampleFormat);
void RemoveCircularBufferSink(nn::audio::AudioRendererConfig* config,
                              nn::audio::CircularBufferSinkType* sink,
                              nn::audio::FinalMixType* mix);
size_t ReadCircularBufferSink(nn::audio::CircularBufferSinkType* sink, void* buffer,
                              size_t buffer_size);
u32 GetSinkNodeId(nn::audio::CircularBufferSinkType const* sink);
size_t
GetRequiredBufferSizeForCircularBufferSink(nn::audio::AudioRendererParameter const* inParameter,
                                           s32 inputCount, s32 frameCount,
                                           nn::audio::SampleFormat sampleFormat);

// Audio Renderer Mix APIs
bool AcquireFinalMix(nn::audio::AudioRendererConfig* config, nn::audio::FinalMixType* mix,
                     s32 bufferCount);
bool AcquireSubMix(nn::audio::AudioRendererConfig* config, nn::audio::SubMixType* mix,
                   s32 sampleRate, s32 bufferCount);
void ReleaseSubMix(nn::audio::AudioRendererConfig* config, nn::audio::SubMixType* mix);
void SetSubMixDestination(nn::audio::AudioRendererConfig* config, nn::audio::SubMixType* source,
                          nn::audio::FinalMixType* destination);
void SetSubMixDestination(nn::audio::AudioRendererConfig* config, nn::audio::SubMixType* source,
                          nn::audio::SubMixType* destination);
void SetSubMixMixVolume(nn::audio::SubMixType* source, nn::audio::FinalMixType* destination,
                        float volume, s32 sourceIndex, s32 destinationIndex);
void SetSubMixMixVolume(nn::audio::SubMixType* source, nn::audio::SubMixType* destination,
                        float volume, s32 sourceIndex, s32 destinationIndex);
u32 GetSubMixNodeId(nn::audio::SubMixType const* mix);

// Audio Renderer Voice APIs
Result AcquireVoiceSlot(nn::audio::AudioRendererConfig* config, nn::audio::VoiceType* voice,
                        s32 sampleRate, s32 channelCount, nn::audio::SampleFormat sampleFormat,
                        s32 priority, void const* buffer, size_t bufferSize);
void ReleaseVoiceSlot(nn::audio::AudioRendererConfig* config, nn::audio::VoiceType* voice);
bool IsVoiceValid(nn::audio::VoiceType const* voice);
bool IsVoiceDroppedFlagOn(nn::audio::VoiceType const* voice);
void ResetVoiceDroppedFlag(nn::audio::VoiceType* voice);
void SetVoiceDestination(nn::audio::AudioRendererConfig* config, nn::audio::VoiceType* voice,
                         nn::audio::FinalMixType* mix);
void SetVoiceDestination(nn::audio::AudioRendererConfig* config, nn::audio::VoiceType* voice,
                         nn::audio::SubMixType* mix);
void SetVoiceVolume(nn::audio::VoiceType* config, float volume);
void SetVoicePitch(nn::audio::VoiceType* config, float pitch);
void SetVoicePlayState(nn::audio::VoiceType* voice, nn::audio::VoiceType::PlayState state);
void SetVoicePriority(nn::audio::VoiceType* voice, s32 priority);
void SetVoiceBiquadFilterParameter(nn::audio::VoiceType* voice, s32 index,
                                   nn::audio::BiquadFilterParameter const& biquadFilterParameter);
void SetVoiceMixVolume(nn::audio::VoiceType* voice, nn::audio::FinalMixType* mix, float volume,
                       int sourceIndex, int destinationIndex);
void SetVoiceMixVolume(nn::audio::VoiceType* voice, nn::audio::SubMixType* mix, float volume,
                       int sourceIndex, int destinationIndex);
nn::audio::VoiceType::PlayState GetVoicePlayState(nn::audio::VoiceType const* voice);
s32 GetVoicePriority(nn::audio::VoiceType const* voice);
u64 GetVoicePlayedSampleCount(nn::audio::VoiceType const* voice);
u32 GetVoiceNodeId(nn::audio::VoiceType const* voice);
bool AppendWaveBuffer(nn::audio::VoiceType* voice, nn::audio::WaveBuffer const* waveBuffer);
nn::audio::WaveBuffer* GetReleasedWaveBuffer(nn::audio::VoiceType* voice);
}  // namespace audio
}  // namespace nn
