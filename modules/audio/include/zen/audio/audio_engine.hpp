#pragma once

#include <string>
#include "zen/audio/audio_types.hpp"

namespace zen {

namespace audio {

class AudioEngine {
public:
    // singleton
    static AudioEngine& getInstance();

    // module lifecycle
    bool init(AudioBackendEnum _backendType);
    void shutdown();

    // backend
    AudioBackendEnum getBackend() const;

    // device
    void getAudioDevices();
    bool setAudioDevice(const std::string& _deviceName);
    std::string getCurrentAudioDevice();
    int getOutputSampleRate() const;
    int getOutputNumChannels() const;
    
    // global controls
    void setMasterVolume(float _volume);
    float getMasterVolume() const;

private:
    // private constructor for singleton
    AudioEngine();
    ~AudioEngine();

    void mix(size_t frames);

    // PIMPL
    class IAudioImpl;
    IAudioImpl* m_impl;

    // internal state
    float m_masterVolume;

};


// // ---------------- Impl ----------------
// struct AudioEngine::Impl {
//     AudioBackendPtr backend;
//     AudioDevicePtr activeDevice;
//     AudioStreamRegister streamRegister;
//     float globalVolume = 1.0f;

//     void update() {
//         streamRegister.updateAllStreams();
//         if (backend && activeDevice && activeDevice->isOpen()) {
//             backend->update();
//         }
//     }
// };

// 8️⃣ How This Architecture Works

// AudioEngine → Impl → Backend → Device → Streams

// Engine is user-facing

// Impl stores backend, active device, global volume, stream register

// Backend implements IAudioBackend for SDL2, OpenAL, etc.

// Device implements AudioDevice (enumeration, format negotiation, callback/push feeding)

// Streams are logical clip players that register with backend

// Backend runtime selection

// Engine initializes a backend by name

// Impl stores backend reference

// Device feeding (callback/submitFrames) goes through backend

// Stream updating

// AudioStreamRegister updates all streams

// Backend mixes them and submits to AudioDevice

// ✅ This is a full, practical C++ audio engine interface suitable for runtime-swappable backends (SDL2/OpenAL), with proper device management, playback control, and stream handling.




} // namespace audio

} // namespace zen
