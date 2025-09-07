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
        // MAIN -> THREAD (window creation)

        // Move jobs into a local queue
        std::queue<std::function<void()>> localQueue;
        { // create a scope so lock is destroyed automatically
            std::lock_guard<std::mutex> lock(jobsMutex);
            std::swap(localQueue, jobs); // take all jobs out atomically
        }

        while (!localQueue.empty()) {
            localQueue.front()(); // run the job
            localQueue.pop(); // remove the job
        }

        // THREAD -> MAIN

        for (auto& [id, window] : windows) {
            while (!window->inputs.empty()) {
                WindowInput input = window->inputs.front();
                window->inputs.pop(); // remove the input

                // Move to main thread Window
                if (auto mainWindow = App::get().getWindowById(id)) {
                    mainWindow->pushInput(input);
                }
            }
        }

        // UPDATE

        for (auto& [id, window] : windows) {
            window->update();
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // don’t busymax
    }
}

void WindowManager::start() {
    if (running) return;
    std::cout << "Starting window manager" << std::endl;
    running = true;
    thread = std::thread(&WindowManager::run, this); // start thread
}

void WindowManager::stop() {
    if (!running) return;
    std::cout << "Stopping window manager" << std::endl;
    running = false;
    if (thread.joinable()) {
        thread.join(); // stop thread
    }
}