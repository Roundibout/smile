#include "theme.hpp"

Color4 Theme::getColor(const ThemeColor& key) const {
    auto it = colors.find(key);
    if (it != colors.end()) return it->second;
    return Color4(1.0f, 0.0f, 1.0f); // fallback to bright magenta to highlight missing keys
}