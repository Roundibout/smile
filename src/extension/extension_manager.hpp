#pragma once

#include <string>
#include <memory>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <nlohmann/json.hpp>

#include <util/logger.hpp>

#include <extension/extension.hpp>

class ExtensionManager {
private:
    ExtensionManager() {
        extensionsFolder = std::filesystem::current_path() / "extensions";
    }
    ExtensionManager(const ExtensionManager&) = delete;
    ExtensionManager& operator=(const ExtensionManager&) = delete;

    std::filesystem::path extensionsFolder;
    Id nextId = 0;
    std::vector<std::unique_ptr<Extension>> extensions;
public:
    static ExtensionManager& get() {
        static ExtensionManager instance; 
        return instance;
    }

    void loadAll();
};