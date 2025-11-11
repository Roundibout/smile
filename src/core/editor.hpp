#pragma once

#include <core/signal.hpp>

#include <util/logger.hpp>

class App;
class Tool;

class Editor {
public:
    enum class Mode {
        Object,
        Edit
    };
private:
    App& app;

    Mode mode = Mode::Object;

    Tool* objectModeTool;
    Tool* editModeTool;
public:
    Signal<Mode> onModeChanged;
    Signal<Tool*, Mode> onToolSelected;

    Editor(App& app) : app(app) {}

    // Getters

    Mode getMode() {return mode;}

    Tool* getSelectedTool(Mode toolMode);

    // Setters

    void setMode(Mode newMode) {
        mode = newMode;
        onModeChanged.emit(mode);
    }

    void setSelectedTool(Mode toolMode, Tool* tool);
};