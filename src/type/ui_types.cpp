#include "ui_types.hpp"

std::string UIDim::toString() const {
    return "(" + std::to_string(scale) + ", " + std::to_string(offset) + ")";
}

std::string UIDim2::toString() const {
    return "{" + x.toString() + ", " + y.toString() + "}";
}