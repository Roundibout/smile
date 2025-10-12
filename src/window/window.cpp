/*
File:
    window.cpp
Authors:
    Lucas
Purpose:
    Implementation of the Window class, used to create windows in Smile
License:
    MIT (see LICENSE file)
*/

#include "window.hpp"

#include <ui/widgets/panels/panel_container.hpp>
#include <ui/widgets/viewport/viewport.hpp>
#include <ui/widgets/console/console.hpp>

Window::Window(const uint32_t& id, const WindowConfig& config)
    #ifdef _WIN32
        : impl(std::make_unique<WindowWin32>(id, config)), // initializer shenanigans
        renderer(impl.get()),
        lastSize(config.size)
    #endif
{
    // maybe do other stuff later
    PanelContainer* container = addWidget<PanelContainer>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0.0), UIDim2(1.0f, 0, 1.0f, 0))));
    PanelLeaf* viewportPanel = static_cast<PanelLeaf*>(container->getPanel());
    viewportPanel->addChild<Viewport>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0.0), UIDim2(1.0f, 0, 1.0f, 0))));
    PanelSplit* split1 = container->splitPanel(PanelSplitDirection::Vertical, 0.8f, PanelSplitPlacement::First);
    PanelSplit* split2 = split1->splitPanel(PanelSplitPlacement::First, PanelSplitDirection::Horizontal, 0.2f, PanelSplitPlacement::Second);
    PanelSplit* split3 = split1->splitPanel(PanelSplitPlacement::Second, PanelSplitDirection::Horizontal, 0.5f, PanelSplitPlacement::First);
    PanelLeaf* consolePanel = static_cast<PanelLeaf*>(split3->getPanel(PanelSplitPlacement::First));
    //consolePanel->addChild<Console>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0.0), UIDim2(1.0f, 0, 1.0f, 0))));
}

void Window::process() {
    // Clean renderer
    renderer.clean();
    bool overdirtied = false;

    // Get inputs through impl update
    inputs = impl->update();

    UIBounds bounds(impl->getSize(), UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));

    // Process inputs
    while (!inputs.empty()) {
        WindowInput& input = inputs.front();

        // Process input on widgets first
        bool consumed = false;
        for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
            if (consumed == false) {
                consumed = (*it)->processWindowInput(input, bounds);
            } else {
                (*it)->observeWindowInput(input, bounds);
            }
        }

        // Find any input callbacks
        auto it = callbacks.find(WindowEvent::Input);
        if (it != callbacks.end()) { // Is there any?
            // Go through all of them and run them
            for (auto& func : it->second) {
                func(input, bounds);
            }
        }

        // Set dirty overrides
        if (input.type == WindowInputType::WindowResized || input.type == WindowInputType::WindowMoved) {
            overdirtied = true;
            lastSize = impl->getSize();
        }

        inputs.pop(); // remove the input
    }

    if (overdirtied == true) {
        renderer.dirty();
    }
}

void Window::update(float deltaTime) {
    // Check for new window size and force render
    Vector2 windowSize = impl->getSize();
    if (lastSize != windowSize) {
        renderer.dirty();
        lastSize = windowSize;
    }
    // Update widgets first
    UIBounds bounds(impl->getSize(), UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));
    for (auto& widget : widgets) {
        widget->update(deltaTime, bounds);
    }
    // Find any update callbacks
    auto it = callbacks.find(WindowEvent::Update);
    if (it != callbacks.end()) { // Is there any?
        // Go through all of them and run them
        for (auto& func : it->second) {
            func(deltaTime, bounds);
        }
    }
}

void Window::render(float uiScale) {
    if (renderer.isDirty() || blank == true) { // Only render if the renderer is dirty or the window is blank
        blank = false;
        // Begin the frame
        renderer.beginFrame(uiScale);
        // Render widgets first
        UIBounds bounds(impl->getSize(), UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));
        for (auto& widget : widgets) {
            widget->render(bounds);
        }
        // Find any render callbacks
        auto it = callbacks.find(WindowEvent::Render);
        if (it != callbacks.end()) { // Is there any?
            // Go through all of them and run them
            for (auto& func : it->second) {
                func(bounds);
            }
        }
        // End the frame
        renderer.endFrame();
    }
}

void Window::connectCallback(WindowEvent event, sol::function callback) {
    callbacks[event].push_back(callback);
}

void Window::setCursor(Cursor cursor) {
    impl->setCursor(cursor);
}

void Window::setCapture() {
    impl->setCapture();
}

void Window::releaseCapture() {
    impl->releaseCapture();
}