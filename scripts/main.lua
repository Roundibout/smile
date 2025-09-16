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
config.title = "Hello"
config.size = Vector2.new(1920, 1080)
config.minSize = Vector2.new(400, 400)
config.color = Theme:GetColor(ThemeColor.WindowBackground)
config.maximized = false

local window = App:CreateWindow(config)

local pressed = true;
local position = Vector2.new(50, 50)
local size = Vector2.new(200, 50)
local color = Color4.new(0.3, 0.3, 0.3, 1)
local hovered = false
local clickTimer = 0

window:ConnectUpdate(function(deltaTime)
    clickTimer = clickTimer - deltaTime
    if clickTimer < 0 then
        clickTimer = 0
    end
end)

window:ConnectRender(function()
    window.renderer:DrawRect(position, size, color)
end)

window:ConnectInput(function(input)
    if input.type == WindowInputType.MouseMove then
        local inside = input.mouse.position.x >= position.x and input.mouse.position.x <= position.x + size.x and input.mouse.position.y >= position.y and input.mouse.position.y <= position.y + size.y
        if hovered ~= inside then
            hovered = inside
            if hovered == true then
                color = Color4.new(0.4, 0.4, 0.4, 1)
            else
                color = Color4.new(0.3, 0.3, 0.3, 1)
            end
            window.renderer:Dirty()
        end
    end
end)

App:Run()