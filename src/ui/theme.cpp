#include "theme.hpp"

Color4 Theme::getColor(const ThemeColor& key) const {
    auto it = colors.find(key);
    if (it != colors.end()) return it->second;
    return Color4(1.0f, 0.0f, 1.0f); // fallback to bright magenta to highlight missing keys
}

float Theme::getMetric(const ThemeMetric& key) const {
    auto it = metrics.find(key);
    if (it != metrics.end()) return it->second;
    return 1.0f; // fallback
}

int Theme::getMetricInt(const ThemeMetric& key) const {
    return static_cast<int>(getMetric(key));
}