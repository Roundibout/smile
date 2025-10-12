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

---@class Theme
Theme = {}

---Returns the specified metric from the theme.
---@param key ThemeMetric
---@return number metric
function Theme:GetMetric(key) end

---Returns the specified color from the theme.
---@param key ThemeColor
---@return Color4 color
function Theme:GetColor(key) end

---Returns the specified font (file path) from the theme.
---@param key ThemeFont
---@return string font
function Theme:GetFont(key) end