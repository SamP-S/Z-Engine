#pragma once

#include <chrono>
#include <cstdint>


namespace zen {

namespace core {


class Timer {
private:
    std::chrono::time_point<std::chrono::system_clock> m_startTime;

public:
    // Constructor
    Timer();

    // Resets timer 0.0s
    void Reset();

    // Gets elapsed time since the timer was started or last reset
    double GetElapsed(bool _isMilli = false);

};


} // namespace core

} // namespace zen
