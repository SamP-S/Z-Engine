#pragma once

#include "zen/core/i_register.hpp"


namespace zen {

namespace audio {

// forward declare
class AudioStream;

class AudioStreamRegister : public core::IRegister<AudioStream> {
public:
    static AudioStreamRegister& getInstance();

protected:
    AudioStreamRegister() = default;
    ~AudioStreamRegister() = default;

};

} // namespace audio

} // namespace zen
