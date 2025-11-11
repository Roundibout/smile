#include "editor.hpp"
#include <core/app.hpp>
#include <extension/tool.hpp>

Tool* Editor::getSelectedTool(Mode toolMode) {
    if (toolMode == Mode::Object) {
        return objectModeTool;
    } else if (toolMode == Mode::Edit) {
        return editModeTool;
    }
    return nullptr;
}

void Editor::setSelectedTool(Mode toolMode, Tool* tool) {
    ToolConfig config = tool->getConfig();
    if (config.mode != toolMode) {
        Logger::warn("Tool \"" + config.name + "\" is not of the specified mode");
        return;
    }
    objectModeTool = tool;
    onToolSelected.emit(tool, toolMode);
}