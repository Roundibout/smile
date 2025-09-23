#pragma once

#include <vector>
#include <memory>

#include <ui/widgets/widget.hpp>

class ContainerWidget : public Widget {
protected:
    std::vector<std::unique_ptr<Widget>> children;
public:
    ContainerWidget(Window* window, UILayout layout) : Widget(window, layout) {}

    virtual void update(float deltaTime) override;
    virtual void render(UIBounds bounds) override;
    virtual void processWindowInput(WindowInput& input) override;

    template <typename T, typename... Args>
    T* addChild(Args&&... args) {
        static_assert(std::is_base_of<Widget, T>::value, "T must be a Widget");

        auto child = std::make_unique<T>(window, std::forward<Args>(args)...);
        T* ptr = child.get(); // Keep raw pointer for return
        children.push_back(std::move(child));

        return ptr;
    }
};