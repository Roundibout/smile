#include "main_window.hpp"
#include <core/app.hpp>

constexpr float UI_SCALE_MAX = 2.0f;
constexpr float UI_SCALE_MIN = 0.5f;

void MainWindow::create() {
    WindowConfig config;
    config.title = "Smile";
    config.size = Vector2(1920, 1080);
    config.minSize = Vector2(400, 400);
    config.color = app.theme.getColor(ThemeColor::WindowBackground);
    config.maximized = true;

    Window* window = app.createWindow(config);

    PanelContainer* container = window->addWidget<PanelContainer>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 30), UIDim2(1.0f, 0, 1.0f, -120))));
    PanelLeaf* viewportPanel = static_cast<PanelLeaf*>(container->getPanel());
    viewportPanel->addChild<Viewport>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));
    PanelSplit* split1 = container->splitPanel(PanelSplitDirection::Vertical, 0.5f, PanelSplitPlacement::First);
    PanelSplit* split2 = split1->splitPanel(PanelSplitPlacement::First, PanelSplitDirection::Horizontal, 0.2f, PanelSplitPlacement::Second);
    PanelSplit* split3 = split1->splitPanel(PanelSplitPlacement::Second, PanelSplitDirection::Horizontal, 0.2f, PanelSplitPlacement::First);
    PanelLeaf* consolePanel = static_cast<PanelLeaf*>(split3->getPanel(PanelSplitPlacement::First));
    consolePanel->addChild<Console>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));
    PanelLeaf* viewport2Panel = static_cast<PanelLeaf*>(split3->getPanel(PanelSplitPlacement::Second));
    viewport2Panel->addChild<Viewport>(UILayout(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, 0))));

    TabBar* tabBar = window->addWidget<TabBar>(UILayout(UIRect(UIDim2(0.0f, 10, 1.0f, -90), UIDim2(1.0f, -20, 0.0f, 50))), TabBarDirection::Horizontal);
    TabId tab1 = tabBar->addTab("Tab1");
    TabId tab2 = tabBar->addTab("Tab2");

    float scale = 1.0f;
    
    window->connectInput([this](WindowInput& input, const UIBounds& bounds) {
        if (input.type == WindowInputType::KeyDown) {
            if (input.key == KeyCode::Equals) {
                app.setUIScale(std::clamp(app.getUIScale() + 0.25f, UI_SCALE_MIN, UI_SCALE_MAX)); 
                Logger::print("Scale +");
            } else if (input.key == KeyCode::Minus) {
                app.setUIScale(std::clamp(app.getUIScale() - 0.25f, UI_SCALE_MIN, UI_SCALE_MAX)); 
                Logger::print("Scale -");
            }
        }
    });
}