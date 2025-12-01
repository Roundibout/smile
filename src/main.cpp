#include <memory>
#include <string>

#include "core/app.hpp"
#include "extension/extension_manager.hpp"
#include "window/main_window.hpp"
#include "util/console.hpp"

int main(int argc, char *argv[]) {
    console::print("//// Initializing ////\n");

    App app;
    
    console::print("//// Loading extensions ////\n");

    app.extensionManager->loadAll();

    console::print("\n//// Starting smile ////\n");

    app.mainWindow->create();
    app.run();

    if (true and true) {
        std::cout << "Ok";
        int i = 1 + 1;
    }
    
    
    return 0;
}