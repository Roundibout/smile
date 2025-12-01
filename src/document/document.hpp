#pragma once

#include <vector>
#include <memory>

#include "type/vector2.hpp"
#include "type/rect.hpp"

#include "document/canvas.hpp"
#include "document/instance.hpp"
#include "document/object.hpp"

class Document {
public:
    std::vector<std::unique_ptr<Canvas>> canvases;
    std::vector<std::unique_ptr<Instance>> instances;

    Document() {}

    Canvas* createCanvas(const Rect& rect);
    Object* createObject();

    std::vector<std::unique_ptr<Instance>>& getContents() {return instances;}
};