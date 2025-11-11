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
    if (tool->getMode() != toolMode) {
        Logger::warn("Tool \"" + tool->getName() + "\" is not of the specified mode");
        return;
    }
    objectModeTool = tool;
    tool->fireEvent(ToolEventType::Select);
    onToolSelected.emit(tool, toolMode);
}