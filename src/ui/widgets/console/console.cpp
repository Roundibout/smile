#include "console.hpp"
#include <window/window.hpp>

void Console::update(float deltaTime, const UIBounds& bounds) {
    if (newEntry) {
        window->renderer.dirty();
    }
}

void Console::render(const UIBounds& bounds) {
    window->renderer.drawText(UIDim2(0.0f, 10, 1.0f, -30), bounds, "Console", Theme::font(ThemeFont::Regular), 20, Color4(1.0f, 1.0f, 1.0f, 0.5f));

    UILayout content(UIRect(UIDim2(0.0f, 0, 0.0f, 0), UIDim2(1.0f, 0, 1.0f, -45)));
    content.cornerRB = UIDim(0.0f, Theme::metric(ThemeMetric::PanelCorner));
    content.cornerLB = UIDim(0.0f, Theme::metric(ThemeMetric::PanelCorner));

    window->renderer.drawRoundedRect(content, bounds, Theme::color(ThemeColor::ConsoleBackground));

    window->renderer.beginStencil();
    window->renderer.drawRoundedRect(content, bounds);
    window->renderer.useStencil();

    float height = window->renderer.resolveLayout(content, bounds).rect.size.y + Theme::metric(ThemeMetric::ConsoleTextSize); // add 20 for one extra line so hanging letters aren't cut
    int absoluteLineHeight = static_cast<int>(window->renderer.scale(Theme::metric(ThemeMetric::ConsoleTextSize)));

    const std::vector<LogEntry>& logs = Logger::getAllLogs();

    int currentHeight = 10;
    for (size_t i = 0; i < logs.size(); ++i) {
        const LogEntry& log = logs[logs.size() - 1 - i];

        Color4 color;
        std::string font;
        if (log.level == LogLevel::Info) {
            color = Theme::color(ThemeColor::ConsoleInfo);
            font = Theme::font(ThemeFont::CodeRegular);
        } else if (log.level == LogLevel::Warning) {
            color = Theme::color(ThemeColor::ConsoleWarning);
            font = Theme::font(ThemeFont::CodeBold);
        } else if (log.level == LogLevel::Error) {
            color = Theme::color(ThemeColor::ConsoleError);
            font = Theme::font(ThemeFont::CodeBold);
        }

        // Get the height which the text will be drawn off of
        int textHeight = 10 + Theme::metricInt(ThemeMetric::ConsoleTextSize) * i;

        // Get the width of the timestamp to offset this log's message
        int timestampWidth = static_cast<int>(FontManager::get().getTextWidth(log.timestamp, Theme::font(ThemeFont::CodeRegular), Theme::metricInt(ThemeMetric::ConsoleTextSize)));
        
        // Split message into lines on each "\n"
        std::vector<std::string> lines;
        std::string current;
        for (char c : log.message) {
            if (c == '\n') {
                lines.push_back(current);
                current.clear();
            } else {
                current += c;
            }
        }
        if (!current.empty()) lines.push_back(current);
        
        // Draw each line
        for (size_t j = 0; j < lines.size(); ++j) {
            int lineHeight = textHeight + Theme::metricInt(ThemeMetric::ConsoleTextSize) * j;

            window->renderer.drawText(
                UIDim2(0.0f, 10 + timestampWidth, 0.0f, lineHeight),
                bounds,
                log.message,
                font,
                Theme::metricInt(ThemeMetric::ConsoleTextSize),
                color
            );

            if (j + 1 == lines.size()) { // Last line (first one)
                // Draw the timestamp
                window->renderer.drawText(
                    UIDim2(0.0f, 10, 0.0f, lineHeight),
                    bounds,
                    log.timestamp,
                    Theme::font(ThemeFont::CodeRegular),
                    Theme::metricInt(ThemeMetric::ConsoleTextSize),
                    Theme::color(ThemeColor::ConsoleTimestamp)
                );
            }

            currentHeight += absoluteLineHeight;
            if (currentHeight >= height) {
                break;
            }
        }
    }

    window->renderer.endStencil();
}

bool Console::processWindowInput(WindowInput&, const UIBounds&) {
    return false;
}

void Console::observeWindowInput(WindowInput&, const UIBounds&) {

}