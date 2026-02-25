#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>

#include "core/timer.hpp"
#include "spdlog/spdlog.h"

using namespace zen::core;

int main() {
    // test timing
    Timer timer;
    timer.Reset();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    double elapsed = timer.GetElapsed();
    std::cout << "Elapsed time: " << elapsed << " seconds\n";
    
    // test spdlog
    spdlog::info("This is an info message");
    spdlog::warn("This is a warning message");
    spdlog::error("This is an error message");

    return 0;
}