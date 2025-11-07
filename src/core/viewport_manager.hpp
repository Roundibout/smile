#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <ui/widgets/tool/category_tool_bar.hpp>

class ViewportManager {
private:
    ViewportManager() {
        toolBar = std::make_unique<CategoryToolBar>(nullptr, UILayout(UIDim2(0.0f, 10, 0.0f, 0), UIDim2(0.0f, 60, 1.0f, -50))); // TODO: this is a weird and dangerous idea. Probably don't do this?
    }
    ViewportManager(const ViewportManager&) = delete;
    ViewportManager& operator=(const ViewportManager&) = delete;
public:
    static ViewportManager& get() {
        static ViewportManager instance; 
        return instance;
    }

    // UI elements
    std::unique_ptr<CategoryToolBar> toolBar; // TODO: this is a weird and dangerous idea. Probably don't do this?
    std::unordered_map<std::string, ToolEntryId> toolToToolBar;
}; 