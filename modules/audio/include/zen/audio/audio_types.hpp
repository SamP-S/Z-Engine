# pragma once

#include <string>
#include <functional>

namespace zen {

namespace audio {

	enum class SampleFormat {
		PCM16,     // 16-bit signed integer
		PCM32,     // 32-bit signed integer
		Float32    // 32-bit float
	};

	enum class StreamType {
		Static,     // loaded into memory
		Stream      // streamed from disk for large tracks
	};

	enum class PlaybackState {
		Stopped,
		Playing,
		Paused
	};

	struct AudioDeviceInfo {
		uint32_t sampleRate;        // actual output rate
		uint16_t numChannels;       // actual channel count
		uint16_t bufferSizeFrames;  // frames per buffer
	};

	using AudioCallback = std::function<void(float* _output, size_t _frameCount)>;

} // namespace audio

} // namespace zen
