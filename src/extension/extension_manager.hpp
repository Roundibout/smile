#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "extension/extension.hpp"
#include "extension/menu_action.hpp"
#include "extension/context_action.hpp"
#include "extension/tool.hpp"
#include "util/console.hpp"

class App;

class ExtensionManager {
private:
    App& app;

    std::filesystem::path extensionsFolder = "extensions";
    Id nextId = 0;
    std::vector<std::unique_ptr<Extension>> extensions;
public:
    explicit ExtensionManager(App& app) : app(app) {}

    void loadAll();
};