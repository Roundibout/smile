---@meta _

---@class App
App = {}

---Begins the app loop.
function App:Run() end

---Stops the app loop and quits the program.
function App:Quit() end

---Creates a window and registers it with the app.
---@param config WindowConfig
---@return Window window The created window object
function App:CreateWindow(config) end