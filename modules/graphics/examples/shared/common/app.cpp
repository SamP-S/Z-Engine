#include "app.hpp"

// std lib
#include <iostream>

// pure virtual destructor must have a definition
App::~App() = default;

// define static members
App* App::s_apps[App::s_kMaxApps] = { nullptr, nullptr };
std::mutex App::s_mtx = std::mutex{};

// app registry
bool App::registerApp(uint32_t _index, App* _app) {
    if (_index >= s_kMaxApps) {
        std::cerr << "[App] ERROR: Cannot register app at index " << _index << ", max apps is " << s_kMaxApps << "." << std::endl;
        return false;
    }

    std::lock_guard<std::mutex> lock(s_mtx);
    if (s_apps[_index] != nullptr) {
        std::cerr << "[App] ERROR: Cannot register app at index " << _index << ", index is already claimed." << std::endl;
        return false;
    }
    s_apps[_index] = _app;
    return true;
}

App* App::getApp(uint32_t _index) {
    if (_index >= s_kMaxApps || _index < 0) {
        std::cerr << "[App] ERROR: Cannot get app at index " << _index << ", max apps is " << s_kMaxApps << "." << std::endl;
        return nullptr;
    }

    std::lock_guard<std::mutex> lock(s_mtx);
    return s_apps[_index];
}
