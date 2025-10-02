#include "ui_types.hpp"

std::string UIDim::to_string() const {
    return "(" + std::to_string(scale) + ", " + std::to_string(offset) + ")";
}

std::string UIDim2::to_string() const {
    return "{" + x.to_string() + ", " + y.to_string() + "}";
}