#pragma once

#include <ui/widgets/panels/panel_container.hpp>
#include <ui/widgets/viewport/viewport.hpp>
#include <ui/widgets/console/console.hpp>
#include <ui/widgets/tab/tab_bar.hpp>

class App;

class MainWindow {
private:
    App& app;

    Window* window;
public:
    MainWindow(App& app) : app(app) {}

    void create();
};