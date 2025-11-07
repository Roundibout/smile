#include "document_manager.hpp"

Document* DocumentManager::getCurrentDocument() {
    return documents[currentDocument].get();
}