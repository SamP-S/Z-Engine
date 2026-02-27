#pragma once

#include <string>
#include "zen/audio/audio_types.hpp"


namespace zen {

namespace audio {

// TODO: add support for streaming audio from disk (for large tracks)

class AudioClip {
public:
    AudioClip(const std::string& _path, StreamType _type = StreamType::Static);
    ~AudioClip();

    int getSampleRate() const;
    size_t getNumChannels() const;
    SampleFormat getFormat() const;
    StreamType getStreamType() const;
    const void* getData() const;
    const size_t getNumSamples() const;
private:
    // properties
    uint32_t m_sampleRate;
    uint16_t m_numChannels;
    SampleFormat m_format;
    StreamType m_streamType;

    // data
    void* m_data;
    size_t m_numSamples;
};

} // namespace audio

} // namespace zen