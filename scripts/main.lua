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

local timer = 0
window:ConnectUpdate(function(deltaTime)
    timer = timer + 1
    --print(timer / 60)
    if timer / 60 >= 10 then
        App:Quit()
        --timer = 0
        --App:CreateWindow("Smile", Vector2.new(800, 600))
        --App:Run()
    end
end)

window:ConnectRender(function()
    --print("render")
end)

window:ConnectInput(function(input)
    if input.type == WindowInputType.MouseButtonDown then
        if input.mouse.button == MouseButton.Left then
            print("left mouse button down")
        elseif input.mouse.button == MouseButton.Right then
            print("right mouse button down")
        end
        print(input.mouse.position)
    elseif input.type == WindowInputType.KeyDown then
        if input.key == KeyCode.A then
            print("press A")
        elseif input.key == KeyCode.Home then
            print("press Home")
        elseif input.key == KeyCode.Alt then
            print("press Alt")
        end
    elseif input.type == WindowInputType.KeyUp then
        if input.key == KeyCode.A then
            print("lift A")
        elseif input.key == KeyCode.Home then
            print("lift Home")
        elseif input.key == KeyCode.Alt then
            print("lift Alt")
        end
    end
end)

App:Run()