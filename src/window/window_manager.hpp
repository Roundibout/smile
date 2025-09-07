#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <thread>
#include <chrono>
#include <atomic>
#include <queue>
#include <mutex>
#include <functional>

#include <datatypes/vector2.hpp>

#include <window/window.hpp>
#include <window/window_impl.hpp>
#include <app/app.hpp>

#ifdef _WIN32
    #include "window_win32.hpp"
#endif

class WindowManager {
private:
    // Private constructor to prevent direct instantiation (there should be only one window manager)
    WindowManager() {
        // do something
    }
    // Delete copy constructor and assignment operator to prevent copying (same reason)
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;

    std::queue<std::function<void()>> jobs; // job queue
    std::mutex jobsMutex; // thing to make it not crash

    std::unordered_map<uint32_t, std::unique_ptr<WindowImpl>> windows;
    uint32_t nextId = 1;

    std::thread thread;
    std::atomic<bool> running{false};

    void run();
public:
    static WindowManager& get() {
        // Initialize window manager only once
        static WindowManager instance; 
        return instance;
    }

    uint32_t createWindow(const std::string& title, const Vector2& size);

    void start();
    void stop();
};