#pragma once

#include <unordered_map>
#include <memory>
#include <cstdint>

#include <document/document.hpp>
#include <document/instance.hpp>
#include <document/object.hpp>

using DocumentId = uint32_t;

class DocumentManager {
private:
    DocumentManager() {
        // Make a document for testing
        std::unique_ptr<Document> document = std::make_unique<Document>();
        Object* obj = document->createObject();
        Id v1 = obj->createPoint(Vector2(0.0f, 0.0f));
        Id v2 = obj->createPoint(Vector2(50.0f, 100.0f));
        Id v3 = obj->createPoint(Vector2(100.0f, 0.0f));
        obj->createLine(v1, v2);
        obj->createLine(v2, v3);
        obj->createLine(v3, v1);
        obj->compute();
        documents[currentDocument] = std::move(document); // Store it as current document, for testing again
    }
    DocumentManager(const DocumentManager&) = delete;
    DocumentManager& operator=(const DocumentManager&) = delete;

    std::unordered_map<DocumentId, std::unique_ptr<Document>> documents;
    DocumentId currentDocument = 0;
public:
    static DocumentManager& get() {
        static DocumentManager instance; 
        return instance;
    }

    Document* getCurrentDocument();
};