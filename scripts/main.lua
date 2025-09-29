--[[
File:
    main.lua
Authors:
    Lucas
Purpose:
    Testing (will be the entry point for the Lua side of Smile)
License:
    MIT (see LICENSE file)
--]]

-- create test windows
local config = WindowConfig.new()
config.title = "Smile"
config.size = Vector2.new(1920, 1080)
config.minSize = Vector2.new(400, 400)
config.color = Theme:GetColor(ThemeColor.WindowBackground)
config.maximized = false

local window = App:CreateWindow(config)

local config2 = WindowConfig.new()
config2.color = Theme:GetColor(ThemeColor.WindowBackground)--Color4.new(0.4, 0.25, 0.62, 1)
local window2 = App:CreateWindow(config2)

local scale = 1

local function clamp(number, lower, upper)
    local clamped = number
    if clamped < lower then
        clamped = lower
    elseif clamped > upper then
        clamped = upper
    end
    return clamped
end

window:ConnectInput(function(input)
    if input.type == WindowInputType.KeyDown then
        if input.key == KeyCode.Minus then
            scale = clamp(scale - 0.25, 0.5, 5)
            print(scale)
            App:SetUIScale(scale)
        elseif input.key == KeyCode.Equals then
            scale = clamp(scale + 0.25, 0.5, 5)
            print(scale)
            App:SetUIScale(scale)
        end
    end
end)

App:Run()