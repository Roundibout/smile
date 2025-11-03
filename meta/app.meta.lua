---@meta _

---@class App
App = {}

---Begins the app loop.
function App:run() end

---Stops the app loop and quits the program.
function App:quit() end

---Creates a window and registers it with the app.
---@param config WindowConfig
---@return Window window The created window object
function App:createWindow(config) end

---Sets the app's UIScale.
---@param scale number
function App:setUIScale(scale) end