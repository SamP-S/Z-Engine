#pragma once

#include <string>


namespace zen {

namespace audio {

class Audio {
public:
    // singleton
    static Audio& getInstance();

    // module lifecycle
    bool init();
    void shutdown();

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
    Audio();
    ~Audio();


    void mix(size_t frames);

    // PIMPL
    class IAudioImpl;
    IAudioImpl* m_impl;

    // internal state
    float m_masterVolume;

};

} // namespace audio

} // namespace zen
