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

enum class ThemeFont {
    Regular,
    Italic,
    Bold,
    BoldItalic
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
        // Initialize default fonts
        fonts[ThemeFont::Regular] = "assets/fonts/Inter_18pt-Regular.ttf";
        fonts[ThemeFont::Italic] = "assets/fonts/Inter_18pt-Italic.ttf";
        fonts[ThemeFont::Bold] = "assets/fonts/Inter_18pt-Bold.ttf";
        fonts[ThemeFont::BoldItalic] = "assets/fonts/Inter_18pt-BoldItalic.ttf";
    }
    // Delete copy constructor and assignment operator to prevent copying (same reason)
    Theme(const Theme&) = delete;
    Theme& operator=(const Theme&) = delete;

    std::unordered_map<ThemeColor, Color4> colors;
    std::unordered_map<ThemeMetric, float> metrics;
    std::unordered_map<ThemeFont, std::string> fonts;
public:
    static Theme& get() {
        static Theme instance;
        return instance;
    }

    float getMetric(const ThemeMetric& key) const;
    int getMetricInt(const ThemeMetric& key) const;
    Color4 getColor(const ThemeColor& key) const;
    std::string getFont(const ThemeFont& key) const;

    static float metric(const ThemeMetric& key) {
        return get().getMetric(key);
    }
    static int metricInt(const ThemeMetric& key) {
        return get().getMetricInt(key);
    }
    static Color4 color(const ThemeColor& key) {
        return get().getColor(key);
    }
    static std::string font(const ThemeFont& key) {
        return get().getFont(key);
    }
};