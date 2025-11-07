#pragma once

#include <vector>
#include <memory>

#include <document/instance.hpp>
#include <document/object.hpp>

class Document {
public:
    std::vector<std::unique_ptr<Instance>> instances;

    Document() {}

    Object* createObject() {
        std::unique_ptr<Object> ptr = std::make_unique<Object>("Object");
        Object* raw = ptr.get();
        instances.push_back(std::move(ptr));
        return raw;
    }
};