#pragma once

#include <unordered_map>
#include <memory>
#include <cstdint>

#include "document/document.hpp"
#include "document/canvas.hpp"
#include "document/instance.hpp"
#include "document/object.hpp"

using DocumentId = uint32_t;

class DocumentManager {
private:
    std::unordered_map<DocumentId, std::unique_ptr<Document>> documents;
    DocumentId currentDocument = 0;
public:
    DocumentManager() {
        // Make a document for testing
        std::unique_ptr<Document> document = std::make_unique<Document>();
        Canvas* canvas = document->createCanvas(Rect(-256, -256, 512, 512));
        documents[currentDocument] = std::move(document); // Store it as current document, for testing again
    }

    Document* getCurrentDocument();
};