#pragma once

#include <string>
#include "zen/audio/audio_types.hpp"

namespace zen {

namespace audio {

class AudioDevice {
public:
    virtual ~AudioDevice() = default;

    // device enumeration
    static std::string* getAvailableDevices(size_t& _numDevices);
    static std::string getDefaultDevice();

    // open/close device
    virtual bool open(
        const std::string& _deviceName, 
        uint32_t _targetSampleRate, 
        uint16_t _targetNumChannels, 
        uint16_t _targetBufferSizeFrames
    ) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    // format
    virtual AudioDeviceInfo getDeviceInfo() const = 0;
    virtual std::string getName() const = 0;

    // playback control
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isPlaying() const = 0;

    // audio feeding
    virtual void setCallback(AudioCallback _callback) = 0;
    virtual void submitFrame(const float* _frameData, size_t _frameCount) = 0;

protected:
    bool m_isOpen = false;

};

} // namespace audio

} // namespace zen
