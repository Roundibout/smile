---@meta _

---@class App
App = {}

---Begins the app loop.
function App:Run() end

---Pauses the app loop.
function App:Pause() end

---Creates a window and registers it with the app.
---@param title string The title of the window (the text on the titlebar)
---@param size Vector2 The size of the window at creation
---@return Window window The created window object
function App:CreateWindow(title, size) end