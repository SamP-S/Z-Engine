#pragma once


namespace zen {

namespace audio {

class AudioSDL {
public:
    AudioSDL();
    ~AudioSDL();

    bool init();
    void shutdown();

    void update(float dt);

private:
    // internal state and resources for SDL audio
};

} // namespace audio

} // namespace zen
