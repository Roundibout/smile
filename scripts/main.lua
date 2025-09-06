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
local window2 = App:CreateWindow("Smile2", Vector2.new(1000, 800))

local timer = 0
window:ConnectCallback(WindowEvent.Update, function(deltaTime)
    timer = timer + 1
    print(timer / 60)
    if timer / 60 >= 10 then
        App:Quit()
        timer = 0
        App:CreateWindow("Smile", Vector2.new(800, 600))
        App:Run()
    end
end)

App:Run()