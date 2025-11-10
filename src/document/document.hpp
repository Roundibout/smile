#pragma once

#include <vector>
#include <memory>

#include <datatypes/vector2.hpp>
#include <datatypes/rect.hpp>

#include <document/canvas.hpp>
#include <document/instance.hpp>
#include <document/object.hpp>

class Document {
public:
    std::vector<std::unique_ptr<Canvas>> canvases;
    std::vector<std::unique_ptr<Instance>> instances;

    Document() {}

    Canvas* createCanvas(const Rect& rect) {
        std::unique_ptr<Canvas> canvas = std::make_unique<Canvas>(rect);
        Canvas* ptr = canvas.get();
        canvases.push_back(std::move(canvas));
        return ptr;
    }

    Object* createObject() {
        std::unique_ptr<Object> object = std::make_unique<Object>("Object");
        Object* ptr = object.get();
        instances.push_back(std::move(object));
        return ptr;
    }
};