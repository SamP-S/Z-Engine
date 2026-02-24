#include "core/tick_timer.hpp"


namespace zen {

namespace core {


TickTimer::TickTimer() {
    Start();
}

void TickTimer::Start() {
    m_startTime = std::chrono::system_clock::now();
    m_lastFrameTime = m_startTime;
    m_running = true;
}

void TickTimer::Stop() {
    m_endTime = std::chrono::system_clock::now();
    m_running = false;
}

void TickTimer::Tick() {
    m_tickCount += 1;
    if (m_running) {
        m_lastFrameTime = std::chrono::system_clock::now();
    }
}

int TickTimer::GetTickCount() {
    return m_tickCount;
}

double TickTimer::GetTickElapsed(bool _isMilli) {
    std::chrono::time_point<std::chrono::system_clock> endTime;
    if (m_running) {
        endTime = std::chrono::system_clock::now();
    } else {
        endTime = m_endTime;
    }
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_lastFrameTime).count();
    return _isMilli ? dt : dt / 1000;
}

double TickTimer::GetTotalElapsed(bool _isMilli) {
    std::chrono::time_point<std::chrono::system_clock> endTime;
    if (m_running) {
        endTime = std::chrono::system_clock::now();
    } else {
        endTime = m_endTime;
    }
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count();
    return _isMilli ? dt : dt / 1000;
}


} // namespace core

} // namespace zen
