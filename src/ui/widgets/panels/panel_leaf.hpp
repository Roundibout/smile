#pragma once

#include <memory>

#include <ui/widgets/widget.hpp>
#include <ui/widgets/panels/panel.hpp>

class PanelLeaf : public Panel {
private:
    std::vector<std::unique_ptr<Widget>> children;
    UILayout applyInsetToLayout(
        const UILayout& childLayout, 
        const PanelAdjacency& adjacency
    );
public:
    PanelLeaf(
        App& app, 
        Window* window, 
        PanelContainer* container
    ) : 
        Panel(app, window, container) 
    {}

    void update(
        float deltaTime, 
        const UIBounds& bounds, 
        PanelAdjacency adjacency
    ) override;
    void render(
        const UIBounds& bounds, 
        PanelAdjacency adjacency
    ) override;

    bool processWindowInput(
        WindowInput& input, 
        const UIBounds& bounds,
         PanelAdjacency adjacency
        ) override;
    void observeWindowInput(
        WindowInput& input, 
        const UIBounds& bounds, 
        PanelAdjacency adjacency
    ) override;

    template <typename T, typename... Args>
    T* addChild(Args&&... args) {
        static_assert(std::is_base_of<Widget, T>::value, "T must be a Widget");

        auto child = std::make_unique<T>(app, window, std::forward<Args>(args)...);
        T* ptr = child.get(); // Keep raw pointer for return
        children.push_back(std::move(child));

        return ptr;
    }
};