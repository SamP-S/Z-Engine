#pragma once

#include "zen/audio/audio_clip.hpp"


namespace zen {

namespace audio {

class AudioStream {
public:
    // constructor auto resgisters the stream from audio
    AudioStream();
    // destructor auto unregisters the stream from audio
    ~AudioStream();

    // set the audio clip to be played by this stream
    // stream does not take ownership of the clip
    void setClip(AudioClip* _clip);
    AudioClip* getClip() const;

    // control playback
    void play();
    void stop();
    void pause();
    bool isPlaying() const;

    // properties
    void setVolume(float _volume);
    float getVolume() const;

    void setPitch(float _pitch);
    float getPitch() const;

    void setLooping(bool _looping);
    bool isLooping() const;

    // cursor is in seconds, set to 0 to restart clip
    void setCursor(double _time);
    double getCursor() const;

    // get length of the clip in seconds
    double getLength() const;

private:
    AudioClip* m_clip;
    PlaybackState m_state;
    float m_volume;
    float m_pitch;
    bool m_looping;
    double m_cursor;

};

} // namespace audio

} // namespace zen

