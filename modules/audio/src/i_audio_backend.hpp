#pragma once

#include <string>
#include "zen/audio/audio_types.hpp"
#include "internal/audio_device.hpp"

namespace zen {

namespace audio {

class IAudioBackend {
    public:
        virtual ~IAudioBackend() = default;
        virtual std::string getName() const = 0;

        // backend lifecycle
        virtual bool init() = 0;
        virtual void shutdown() = 0;

        // query devices
        virtual AudioDevice** getAvailableDevices(size_t& _numDevices) const = 0;
        
        // device management
        virtual bool setActiveDevice(AudioDevice* _deviceInfo) = 0;
        virtual AudioDevice* getActiveDevice() const = 0;

        // push mixed audio to device
        virtual void update() = 0;

};

}

}
