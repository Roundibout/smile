#pragma once

#include <unordered_map>
#include <string>

#include <datatypes/color4.hpp>

enum class ThemeColor {
    WindowBackground,
};

class Theme {
private:
    // Private constructor to prevent direct instantiation (there should be only one theme)
    Theme() {
        // Initialize default colors
        colors[ThemeColor::WindowBackground] = Color4(0.05f, 0.05f, 0.05f); 
    }
    // Delete copy constructor and assignment operator to prevent copying (same reason)
    Theme(const Theme&) = delete;
    Theme& operator=(const Theme&) = delete;

    std::unordered_map<ThemeColor, Color4> colors;
public:
    static Theme& get() {
        static Theme instance;
        return instance;
    }

    Color4 getColor(const ThemeColor& key) const;
};