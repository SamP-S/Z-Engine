#include "zen/core/timer.hpp"


namespace zen {

namespace core {


Timer::Timer() {
    Reset();
}

void Timer::Reset() {
    m_startTime = std::chrono::system_clock::now();
}

double Timer::GetElapsed(bool _isMilli) {
    std::chrono::time_point<std::chrono::system_clock> endTime = std::chrono::system_clock::now();
    double dt = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count();
    return _isMilli ? dt : dt / 1000;
}


} // namespace core

} // namespace zen
