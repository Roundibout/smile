#pragma once

class App;

class Editor {
public:
    enum class Mode {
        Object,
        Edit
    };
private:
    App& app;

    Mode mode;
public:
    Editor(App& app) : app(app) {}

    Mode getMode() {return mode;}
};