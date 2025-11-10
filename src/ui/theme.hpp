#pragma once

#include <unordered_map>
#include <string>

#include <datatypes/color4.hpp>

enum class ThemeMetric {
    PanelMargin,
    PanelCorner,
    PanelStroke,

    ConsoleTextSize
};

enum class ThemeColor {
    Accent,
    AccentHighlight,
    Invalid,

    WindowBackground,

    Button,
    ButtonStroke,
    ButtonHovered,
    ButtonStrokeHovered,

    Panel,
    PanelStroke,

    ProjectTab,
    ProjectTabStroke,
    ProjectTabSelected,

    Cursor,
    CursorStroke,

    ViewportBackground,
    ViewportRotationIndicator,
    ViewportRotationIndicatorArrow,

    ConsoleBackground,
    ConsoleTimestamp,
    ConsoleInfo,
    ConsoleWarning,
    ConsoleError,
    ConsoleExtension
};

enum class ThemeFont {
    Regular,
    Italic,
    Bold,
    BoldItalic,

    CodeRegular,
    CodeBold
};

class Theme {
private:
    std::unordered_map<ThemeColor, Color4> colors;
    std::unordered_map<ThemeMetric, float> metrics;
    std::unordered_map<ThemeFont, std::string> fonts;
public:
    Theme() {
        // Initialize default metrics
        metrics[ThemeMetric::PanelMargin] = 10.0f;
        metrics[ThemeMetric::PanelCorner] = 6.0f;
        metrics[ThemeMetric::PanelStroke] = 2.0f;

        metrics[ThemeMetric::ConsoleTextSize] = 20.0f;

        // Initialize default colors
        colors[ThemeColor::Accent] = Color4(0.5f, 0.2f, 0.2f);
        colors[ThemeColor::AccentHighlight] = Color4(0.9f, 0.5f, 0.5f);
        colors[ThemeColor::Invalid] = Color4(1.0f, 0.5f, 0.5f);

        colors[ThemeColor::WindowBackground] = Color4(0.06f, 0.06f, 0.06f);
        colors[ThemeColor::Panel] = Color4(0.2f, 0.2f, 0.2f);
        colors[ThemeColor::PanelStroke] = Color4(0.3f, 0.3f, 0.3f);

        colors[ThemeColor::Button] = Color4(0.08f, 0.08f, 0.08f);
        colors[ThemeColor::ButtonStroke] = Color4(0.1f, 0.1f, 0.1f);
        colors[ThemeColor::ButtonHovered] = Color4(0.14f, 0.14f, 0.14f);
        colors[ThemeColor::ButtonStrokeHovered] = Color4(0.18f, 0.18f, 0.18f);

        colors[ThemeColor::ProjectTab] = Color4(0.07f, 0.07f, 0.07f);
        colors[ThemeColor::ProjectTabStroke] = Color4(0.2f, 0.2f, 0.2f);
        colors[ThemeColor::ProjectTabSelected] = Color4(0.08f, 0.08f, 0.08f);

        colors[ThemeColor::Cursor] = Color4(1.0f, 1.0f, 1.0f);
        colors[ThemeColor::CursorStroke] = Color4(0.0f, 0.0f, 0.0f);

        colors[ThemeColor::ViewportBackground] = Color4(0.15f, 0.15f, 0.15f);
        colors[ThemeColor::ViewportRotationIndicator] = Color4(0.8f, 0.8f, 0.8f, 0.2f); 
        colors[ThemeColor::ViewportRotationIndicatorArrow] = Color4(0.7f, 0.7f, 0.7f);

        colors[ThemeColor::ConsoleBackground] = Color4(0.1f, 0.1f, 0.1f);
        colors[ThemeColor::ConsoleTimestamp] = Color4(0.5f, 0.5f, 0.5f);
        colors[ThemeColor::ConsoleInfo] = Color4(0.9f, 0.9f, 0.9f);
        colors[ThemeColor::ConsoleWarning] = Color4(0.9f, 0.6f, 0.1f);
        colors[ThemeColor::ConsoleError] = Color4(0.9f, 0.1f, 0.05f);
        colors[ThemeColor::ConsoleExtension] = Color4(0.7f, 0.6f, 1.0f);

        // Initialize default fonts
        fonts[ThemeFont::Regular] = "assets/fonts/Inter_18pt-Regular.ttf";
        fonts[ThemeFont::Italic] = "assets/fonts/Inter_18pt-Italic.ttf";
        fonts[ThemeFont::Bold] = "assets/fonts/Inter_18pt-Bold.ttf";
        fonts[ThemeFont::BoldItalic] = "assets/fonts/Inter_18pt-BoldItalic.ttf";
        fonts[ThemeFont::CodeRegular] = "assets/fonts/CourierPrime-Regular.ttf";
        fonts[ThemeFont::CodeBold] = "assets/fonts/CourierPrime-Bold.ttf";
    }

    float getMetric(const ThemeMetric& key) const;
    int getMetricInt(const ThemeMetric& key) const;
    Color4 getColor(const ThemeColor& key) const;
    std::string getFont(const ThemeFont& key) const;
};