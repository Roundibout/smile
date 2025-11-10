#include "extension_manager.hpp"
#include <core/app.hpp>

void ExtensionManager::loadAll() {
    if (extensionsFolder.empty())
        return;

    for (auto& entry : std::filesystem::directory_iterator(extensionsFolder)) {
        // Skip anything that isn't a folder
        if (!entry.is_directory()) continue; 

        std::string extensionPath = entry.path().string();

        // Check if there is an entry point
        std::filesystem::path mainPath = entry.path() / "main.lua";
        if (!std::filesystem::exists(mainPath)) {
            Logger::error("Failed to load extension " + extensionPath + "\n    Doesn't contain main.lua entry point");
            continue;
        }

        // Check if there is a metadata file
        std::filesystem::path metaPath = entry.path() / "meta.json";
        if (!std::filesystem::exists(metaPath)) {
            Logger::error("Failed to load extension " + extensionPath + "\n    Doesn't contain meta.json metadata file");
            continue;
        }

        // Read and parse the JSON
        nlohmann::json metaJson;
        try {
            std::ifstream file(metaPath);
            file >> metaJson;
        } catch (const std::exception& e) {
            Logger::error("Failed to load extension " + extensionPath + "\n    Failed to parse meta.json: " + std::string(e.what()));
            continue;
        }

        // Get the name, required
        std::string extensionName;
        try {
            extensionName = metaJson.at("name").get<std::string>(); // throws if missing
        } catch (const std::exception&) {
            Logger::error("Failed to load extension " + extensionPath + "\n    meta.json missing required field \"name\"");
            continue;
        }
        
        try {
            auto extension = std::make_unique<Extension>(app, nextId, extensionName, extensionPath);
            if (extension->load()) {
                extensions.push_back(std::move(extension));
                ++nextId;
            }
        } catch (const std::exception& err) {
            Logger::error("Failed to load extension " + extensionPath + "\n    ", std::string(err.what()).substr(5));
        }
    }
}