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
local window = App:CreateWindow("Smile", Vector2.new(800, 600))
App:CreateWindow("Smile", Vector2.new(200, 200))
App:CreateWindow("Smile", Vector2.new(400, 600))

local timer = 0
window:ConnectUpdate(function(deltaTime)
    timer = timer + deltaTime
    print(timer)
end)

window:ConnectRender(function()
    --print("render")
end)

window:ConnectInput(function(input)
    if input.type == WindowInputType.MouseScroll then
        print(input.mouse.scroll)
        print(input.mouse.position)
    elseif input.type == WindowInputType.MouseButtonDown then
        print(input.mouse.position)
    end
end)

App:Run()