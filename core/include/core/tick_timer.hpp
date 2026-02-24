#pragma once

#include <chrono>
#include <cstdint>


namespace zen {

namespace core {


class TickTimer
{
private:
    std::chrono::time_point<std::chrono::system_clock> m_startTime;
    std::chrono::time_point<std::chrono::system_clock> m_endTime;
    std::chrono::time_point<std::chrono::system_clock> m_lastFrameTime;
    uint32_t m_tickCount = 0;
    bool m_running = true;

public:
    // constructor
    TickTimer();

    // control
    void Start();
    void Stop();
    void Tick();
    int GetTickCount();
    double GetTickElapsed(bool _isMilli = false);
    double GetTotalElapsed(bool _isMilli = false);

};


} // namespace core

} // namespace zen