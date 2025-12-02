#pragma once

#include <unordered_map>
#include <memory>
#include <chrono>
#include <thread>

#include "type/vector2.hpp"

#include "window/window.hpp"
#include "window/main_window.hpp"
#include "extension/extension_manager.hpp"
#include "core/editor.hpp"
#include "ui/theme.hpp"
#include "document/document_manager.hpp"
#include "util/console.hpp"

class Window; // forward declaration

class App {
private:
    bool running = false;
    std::chrono::time_point<std::chrono::steady_clock> previousTime = std::chrono::steady_clock::now();

    bool needsRender = false;
    float uiScale = 1.0f;

    std::unordered_map<uint32_t, std::unique_ptr<Window>> windows;
    uint32_t nextId = 1;
public:
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<ExtensionManager> extensionManager;
    std::unique_ptr<ExtensionRegistry> extensionRegistry;
    std::unique_ptr<Editor> editor;

    Theme theme;
    DocumentManager documents;

    App() : 
        mainWindow(std::make_unique<MainWindow>(*this)),
        extensionManager(std::make_unique<ExtensionManager>(*this)),
        extensionRegistry(std::make_unique<ExtensionRegistry>(*this)),
        editor(std::make_unique<Editor>(*this))
    {}

    Window* createWindow(const WindowConfig& config);
    Window* getWindowById(const uint32_t& id);

    void destroyWindow(const uint32_t& id);

    void forceRender();

    float getUIScale();
    void setUIScale(float scale);

    void step();

    void run();
    void quit();
};