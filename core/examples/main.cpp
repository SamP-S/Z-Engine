#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>

#include "zen/core/timer.hpp"
#include "zen/core/uuid.hpp"
#include "spdlog/spdlog.h"

using namespace zen::core;

int main() {
    // test timing
    Timer timer;
    timer.Reset();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    double elapsed = timer.GetElapsed();
    std::cout << "Elapsed time: " << elapsed << " seconds\n";
    
    // test uuid
    UUID uuid1;
    UUID uuid2;
    std::cout << "UUID 1: " << static_cast<int64_t>(uuid1) << "\n";
    std::cout << "UUID 2: " << static_cast<int64_t>(uuid2) << "\n";

    // test spdlog
    spdlog::info("This is an info message");
    spdlog::warn("This is a warning message");
    spdlog::error("This is an error message");

    return 0;
}