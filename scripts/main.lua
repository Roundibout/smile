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

App:Run()