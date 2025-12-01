#include "document/document.hpp"

Canvas* Document::createCanvas(const Rect& rect) {
    std::unique_ptr<Canvas> canvas = std::make_unique<Canvas>(rect);
    Canvas* ptr = canvas.get();
    canvases.push_back(std::move(canvas));
    return ptr;
}

Object* Document::createObject() {
    std::unique_ptr<Object> object = std::make_unique<Object>("Object");
    Object* ptr = object.get();
    instances.push_back(std::move(object));
    return ptr;
}