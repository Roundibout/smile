#pragma once

#include <unordered_map>
#include <string>

#include <datatypes/color4.hpp>

enum class ThemeMetric {
    PanelMargin,
    PanelCorner,
    PanelStroke
};

enum class ThemeColor {
    WindowBackground,

    Panel,
    PanelStroke
};

class Theme {
private:
    // Private constructor to prevent direct instantiation (there should be only one theme)
    Theme() {
        // Initialize default metrics
        metrics[ThemeMetric::PanelMargin] = 10.0f;
        metrics[ThemeMetric::PanelCorner] = 6.0f;
        metrics[ThemeMetric::PanelStroke] = 2.0f;
        // Initialize default colors
        colors[ThemeColor::WindowBackground] = Color4(0.07f, 0.07f, 0.07f);
        colors[ThemeColor::Panel] = Color4(0.15f, 0.15f, 0.15f);
        colors[ThemeColor::PanelStroke] = Color4(0.2f, 0.2f, 0.2f);
    }
    // Delete copy constructor and assignment operator to prevent copying (same reason)
    Theme(const Theme&) = delete;
    Theme& operator=(const Theme&) = delete;

    std::unordered_map<ThemeColor, Color4> colors;
    std::unordered_map<ThemeMetric, float> metrics;
public:
    static Theme& get() {
        static Theme instance;
        return instance;
    }

    Color4 getColor(const ThemeColor& key) const;
    float getMetric(const ThemeMetric& key) const;
    int getMetricInt(const ThemeMetric& key) const;
};