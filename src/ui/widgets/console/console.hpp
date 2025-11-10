#pragma once

#include <string>
#include <deque>

#include <ui/widgets/widget.hpp>

#include <util/logger.hpp>

class Console : public Widget {
private:
    bool newEntry = false;
public:
    Console(App& app, Window* window, UILayout layout) : Widget(app, window, layout) {
        Logger::addCallback([this](const LogEntry& log){
            this->newEntry = true;
        });
    }
    
    void update(float deltaTime, const UIBounds& bounds) override;

    void render(const UIBounds& bounds) override;

    bool processWindowInput(WindowInput&, const UIBounds&) override;
    void observeWindowInput(WindowInput&, const UIBounds&) override;
};