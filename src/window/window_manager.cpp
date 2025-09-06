#include "window_manager.hpp"

uint32_t WindowManager::createWindow(const std::string& title, const Vector2& size) {
    uint32_t id = nextId;
    nextId++;

    std::lock_guard<std::mutex> lock(jobsMutex);
    // Push window creation lambda into the queue
    jobs.push([this, title, size, id](){
        #ifdef _WIN32
            std::unique_ptr<WindowImpl> window = std::make_unique<WindowWin32>(title, size);
            windows[id] = std::move(window);
        #endif
    });

    return id;
}

void WindowManager::run() {
    while (running) {
        // Move jobs into a local queue
        std::queue<std::function<void()>> localQueue;
        {
            std::lock_guard<std::mutex> lock(jobsMutex);
            std::swap(localQueue, jobs); // take all jobs out atomically
        }

        while (!localQueue.empty()) {
            localQueue.front()(); // run the job
            localQueue.pop();
        }

        for (auto& [id, window] : windows) {
            window->update();
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // don’t busymax
    }
}

void WindowManager::start() {
    if (running) return;
    running = true;
    std::cout << "starting thread" << std::endl;
    thread = std::thread(&WindowManager::run, this); // start thread
}

void WindowManager::stop() {
    if (!running) return;
    running = false;
    if (thread.joinable()) {
        thread.join(); // stop thread
    }
}