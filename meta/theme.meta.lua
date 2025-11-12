---@meta _

---@enum ThemeMetric
ThemeMetric = {
    PanelMargin = 0,
    PanelCorner = 1,
    PanelStroke = 2
}

---@enum ThemeColor
ThemeColor = {
    WindowBackground = 0,

    Panel = 1,
    PanelStroke = 2
}

---@enum ThemeFont
ThemeFont = {
    Regular = 0,
    Italic = 1,
    Bold = 2,
    BoldItalic = 3,

    CodeRegular = 4,
    CodeBold = 5
}

---@class theme
theme = {}

---Returns the specified metric from the theme.
---@param key ThemeMetric
---@return number metric
function theme:getMetric(key) end

---Returns the specified color from the theme.
---@param key ThemeColor
---@return Color4 color
function theme:getColor(key) end

---Returns the specified font (file path) from the theme.
---@param key ThemeFont
---@return string font
function theme:getFont(key) end