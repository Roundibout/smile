#include "extension.hpp"
#include <core/app.hpp>

Extension::Extension(App& app, Id id, std::string name, std::string path) : app(app), id(id), name(name), folder(path) {
    lua.open_libraries(
        sol::lib::base,
        sol::lib::package,
        sol::lib::string,
        sol::lib::table,
        sol::lib::math
    );

    // Set package path to the extension folder
    std::string luaPackagePath = std::filesystem::path(folder).generic_string() + "/?.lua";
    lua["package"]["path"] = lua["package"]["path"].get<std::string>() + ";" + luaPackagePath;

    // Set console functions
    lua.set_function("print", [this](sol::variadic_args args) {
        std::string out = "[" + this->name + "]";
        for (auto v : args)
            out += " " + v.get<std::string>();
        Logger::log(LogLevel::Extension, out);
    });
    lua.set_function("warn", [this](sol::variadic_args args) {
        std::string out = "[" + this->name + "]";
        for (auto v : args)
            out += " " + v.get<std::string>();
        Logger::warn(out);
    });

    // Bind usertypes

    // -- Enums -- //

    lua.new_enum<WindowEvent>("WindowEvent",
        {
            {"Update", WindowEvent::Update},
            {"Render", WindowEvent::Render},
            {"Input", WindowEvent::Input}
        }
    );

    // Window Input

    lua.new_enum<WindowInputType>("WindowInputType",
        {
            {"MouseButtonDown", WindowInputType::MouseButtonDown},
            {"MouseButtonUp", WindowInputType::MouseButtonUp},
            {"MouseMove", WindowInputType::MouseMove},
            {"MouseScroll", WindowInputType::MouseScroll},
            {"KeyDown", WindowInputType::KeyDown},
            {"KeyUp", WindowInputType::KeyUp},
            {"WindowResized", WindowInputType::WindowResized},
            {"WindowMoved", WindowInputType::WindowMoved},
            {"WindowClosed", WindowInputType::WindowClosed}
        }
    );

    lua.new_enum<MouseButton>("MouseButton",
        {
            {"Left", MouseButton::Left},
            {"Right", MouseButton::Right},
            {"Middle", MouseButton::Middle}
        }
    );

    // Use macro to easily copy over the same items for the enum
    lua.new_enum<KeyCode>("KeyCode", {
    #define X(name) std::make_pair(#name, KeyCode::name),
        KEY_CODE_LIST
    #undef X
    });

    // Theme
    lua.new_enum<ThemeColor>("ThemeColor",
        {
            {"WindowBackground", ThemeColor::WindowBackground}
        }
    );

    // UI
    lua.new_enum<UIStrokeAlignment>("UIStrokeAlignment",
        {
            {"Outside", UIStrokeAlignment::Outside},
            {"Middle", UIStrokeAlignment::Middle},
            {"Inside", UIStrokeAlignment::Inside}
        }
    );

    // Extensions
    lua.new_enum<Editor::Mode>("EditorMode",
        {
            {"Object", Editor::Mode::Object},
            {"Edit", Editor::Mode::Edit}
        }
    );
    lua.new_enum<ToolCategory>("ToolCategory",
        {
            {"Select", ToolCategory::Select},
            {"Transform", ToolCategory::Transform},
            {"Add", ToolCategory::Add},
            {"Modify", ToolCategory::Modify},
            {"Custom", ToolCategory::Custom}
        }
    );
    lua.new_enum<ToolEventType>("ToolEvent",
        {
            {"Select", ToolEventType::Select},
            {"Deselect", ToolEventType::Deselect},
            {"LeftMouseDown", ToolEventType::LeftMouseDown},
            {"LeftMouseUp", ToolEventType::LeftMouseUp},
            {"RightMouseDown", ToolEventType::RightMouseDown},
            {"RightMouseUp", ToolEventType::RightMouseUp}
        }
    );

    // -- Datatypes -- //

    // Vector2
    lua.new_usertype<Vector2>("Vector2",
        // Constructors
        sol::constructors<
            Vector2(), 
            Vector2(float, float)
        >(),
        // Fields
        "x", &Vector2::x,
        "y", &Vector2::y,
        // Operators
        sol::meta_function::addition, &Vector2::operator+,
        sol::meta_function::subtraction, sol::resolve<Vector2(const Vector2&) const>(&Vector2::operator-),
        sol::meta_function::multiplication, &Vector2::operator*,
        sol::meta_function::division, &Vector2::operator/,
        sol::meta_function::floor_division, &Vector2::idiv,
        sol::meta_function::modulus, &Vector2::operator%,
        sol::meta_function::power_of, &Vector2::pow,
        sol::meta_function::equal_to, &Vector2::operator==,
        sol::meta_function::unary_minus, sol::resolve<Vector2() const>(&Vector2::operator-)
    );

    // Rect
    lua.new_usertype<Rect>("Rect",
        // Constructors
        sol::constructors<
            Rect(), 
            Rect(Vector2, Vector2),
            Rect(float, float, float, float)
        >(),
        // Fields
        "position", &Rect::position,
        "size", &Rect::size,
        // Operators
        sol::meta_function::equal_to, &Rect::operator==
    );

    // Color4
    lua.new_usertype<Color4>("Color4",
        // Constructors
        sol::constructors<
            Color4(), 
            Color4(float, float, float),
            Color4(float, float, float, float)
        >(),
        // Fields
        "r", &Color4::r,
        "g", &Color4::g,
        "b", &Color4::b,
        "a", &Color4::a
    );

    // UIDim
    lua.new_usertype<UIDim>("UIDim",
        // Constructors
        sol::constructors<
            UIDim(),
            UIDim(float, int)
        >(),
        // Fields
        "scale", &UIDim::scale,
        "offset", &UIDim::offset,
        // Operators
        sol::meta_function::addition, &UIDim::operator+,
        sol::meta_function::subtraction, &UIDim::operator-,
        sol::meta_function::equal_to, &UIDim::operator==
    );

    // UIDim2
    lua.new_usertype<UIDim2>("UIDim2",
        // Constructors
        sol::constructors<
            UIDim2(),
            UIDim2(const UIDim&, const UIDim&),
            UIDim2(float, int, float, int)
        >(),
        // Fields
        "x", &UIDim2::x,
        "y", &UIDim2::y,
        // Operators
        sol::meta_function::addition, &UIDim2::operator+,
        sol::meta_function::subtraction, &UIDim2::operator-,
        sol::meta_function::equal_to, &UIDim2::operator==
    );
    
    // UIRect
    lua.new_usertype<UIRect>("UIRect",
        // Constructors
        sol::constructors<
            UIRect(),
            UIRect(const UIDim2&, const UIDim2&)
        >(),
        // Fields
        "position", &UIRect::position,
        "size", &UIRect::size,
        // Operators
        sol::meta_function::equal_to, &UIRect::operator==
    );

    // AbsoluteLayout
    lua.new_usertype<AbsoluteLayout>("AbsoluteLayout",
        // Constructors
        sol::constructors<
            AbsoluteLayout(),
            AbsoluteLayout(const Rect&)
        >(),
        // Fields
        "rect", &AbsoluteLayout::rect,
        "cornerRT", &AbsoluteLayout::cornerRT,
        "cornerLT", &AbsoluteLayout::cornerLT,
        "cornerRB", &AbsoluteLayout::cornerRB,
        "cornerLB", &AbsoluteLayout::cornerLB
    );

    // UILayout
    lua.new_usertype<UILayout>("UILayout",
        // Constructors
        sol::constructors<
            UILayout(),
            UILayout(const UIRect&)
        >(),
        // Fields
        "rect", &UILayout::rect,
        "cornerRT", &UILayout::cornerRT,
        "cornerLT", &UILayout::cornerLT,
        "cornerRB", &UILayout::cornerRB,
        "cornerLB", &UILayout::cornerLB,
        // Methods
        "setCorners", &UILayout::setCorners
    );

    // UIBounds
    lua.new_usertype<UIBounds>("UIBounds",
        // Constructors
        sol::constructors<
            UIBounds(),
            UIBounds(const Vector2&),
            UIBounds(const Vector2&, const UILayout&)
        >(),
        // Fields
        "absolute", &UIBounds::absolute,
        "layout", &UIBounds::layout
    );

    // Window Config
    lua.new_usertype<WindowConfig>("WindowConfig",
        // Constructor
        sol::constructors<WindowConfig()>(),
        // Fields
        "title", &WindowConfig::title,
        "size", &WindowConfig::size,
        "color", &WindowConfig::color,
        "resizable", &WindowConfig::resizable,
        "minSize", &WindowConfig::minSize,
        "maxSize", &WindowConfig::maxSize,
        "maximized", &WindowConfig::maximized,
        "minimized", &WindowConfig::minimized,
        "maximizable", &WindowConfig::maximizable,
        "minimizable", &WindowConfig::minimizable
    );

    // Window Input
    lua.new_usertype<MouseInput>("MouseInput",
        "button", &MouseInput::button,
        "position", sol::property(
            [](MouseInput& self) -> Vector2& {return self.position;},
            [](MouseInput& self, Vector2 position) {self.position = position;}
        ), // nested struct (with setter)
        "delta", sol::property(
            [](MouseInput& self) -> Vector2& {return self.delta;},
            [](MouseInput& self, Vector2 delta) {self.delta = delta;}
        ), // same
        "scroll", sol::property(
            [](MouseInput& self) -> Vector2& {return self.scroll;},
            [](MouseInput& self, Vector2 scroll) {self.scroll = scroll;}
        ) // same
    );

    lua.new_usertype<WindowInput>("WindowInput",
        "type", &WindowInput::type,
        "mouse", sol::property([](WindowInput& self) -> MouseInput& {return self.mouse;}), // nested struct
        "key", &WindowInput::key
    );

    // Extension contribution definitions
    lua.new_usertype<MenuActionDefinition>("MenuActionDefinition",
        "name", &MenuActionDefinition::name
    );
    lua.new_usertype<ContextActionDefinition>("ContextActionDefinition",
        "name", &ContextActionDefinition::name
    );
    lua.new_usertype<ToolDefinition>("ToolDefinition",
        "name", &ToolDefinition::name,
        "mode", &ToolDefinition::mode,
        "category", &ToolDefinition::category,
        "connect", [](ToolDefinition& self, ToolEventType type, sol::function func) {
            self.connect(type, func);
        }
    );

    // Document
    lua.new_usertype<Canvas>("Canvas");
    lua.new_usertype<Object>("Object",
        "createPoint", &Object::createPoint,
        "createLine", &Object::createLine
    );

    // -- Classes -- //

    lua.new_usertype<Renderer>("Renderer",
        "dirty", &Renderer::dirty,
        "isDirty", &Renderer::isDirty,

        "resolvePosition", &Renderer::resolvePosition,
        "resolveLayout", &Renderer::resolveLayout,
        "applyLayout", &Renderer::applyLayout,

        "drawTriangle", &Renderer::drawTriangle,
        "drawQuad", &Renderer::drawQuad,
        "drawRect", &Renderer::drawRect,
        "drawStrokeRect", &Renderer::drawStrokeRect,
        "drawRoundedRect", &Renderer::drawRoundedRect,
        "drawRoundedRect", &Renderer::drawRoundedStrokeRect,
        "drawText", &Renderer::drawText
    );

    lua.new_usertype<Window>("Window",
        "renderer", sol::readonly_property([](Window& self) -> Renderer& {return self.renderer;}), // nested struct

        "connectUpdate", [](Window& self, sol::function luaFunc) {
            self.connectUpdate([luaFunc](float deltaTime, const UIBounds& bounds) {
                sol::protected_function_result result = luaFunc(deltaTime, bounds);
                if (!result.valid()) {
                    sol::error err = result;
                    Logger::error("Lua update callback error: ", err.what());
                }
            });
        },

        "connectRender", [](Window& self, sol::function luaFunc) {
            self.connectRender([luaFunc](const UIBounds& bounds) {
                sol::protected_function_result result = luaFunc(bounds);
                if (!result.valid()) {
                    sol::error err = result;
                    Logger::error("Lua render callback error: ", err.what());
                }
            });
        },

        "connectInput", [](Window& self, sol::function luaFunc) {
            self.connectInput([luaFunc](WindowInput& input, const UIBounds& bounds) {
                sol::protected_function_result result = luaFunc(input, bounds);
                if (!result.valid()) {
                    sol::error err = result;
                    Logger::error("Lua input callback error: ", err.what());
                }
            });
        }
    );

    lua.new_usertype<Document>("Document",
        "createCanvas", &Document::createCanvas,
        "createObject", &Document::createObject
    );

    lua.new_usertype<Extension>("Extension",
        "registerMenuAction", &Extension::registerMenuAction,
        "registerContextAction", &Extension::registerContextAction,
        "registerTool", &Extension::registerTool
    );

    // -- Main systems -- //

    lua.new_usertype<App>("App",
        "setUIScale", &App::setUIScale
    );

    lua.new_usertype<DocumentManager>("DocumentManager",
        "current", sol::readonly_property([](DocumentManager& self) -> Document* {return self.getCurrentDocument();})
    );

    lua.new_usertype<Theme>("Theme",
        "getMetric", &Theme::getMetric,
        "getColor", &Theme::getColor,
        "getFont", &Theme::getFont
    );

    // Assign stuff to global variables
    lua["extension"] = this;
    lua["app"] = &app;
    lua["documents"] = &app.documents;
    lua["theme"] = &app.theme;
}

bool Extension::load() {
    Logger::print("Loading extension ", id);

    sol::load_result script = lua.load_file(folder + "/main.lua");
    if (!script.valid()) { // Check if main.lua exists
        sol::error err = script;
        Logger::error("Failed to load extension ", name, "\n    ", err.what()); // Syntax/load error
        return false;
    }

    // Run it safely
    sol::protected_function_result result = script();
    if (!result.valid()) {
        sol::error err = result;
        Logger::error("Failed to load extension ", name, "\n    ", err.what()); // Runtime error
        return false;
    } else {
        Logger::print("Successfully loaded extension \"", name, "\"");
    }

    return true;
}

void Extension::registerMenuAction(MenuActionDefinition definition) {
    MenuAction* action = app.extensionRegistry->registerMenuAction(definition);
    menuActions.push_back(action);
}

void Extension::registerContextAction(ContextActionDefinition definition) {
    ContextAction* action = app.extensionRegistry->registerContextAction(definition);
    contextActions.push_back(action);
}

void Extension::registerTool(ToolDefinition definition) {
    Tool* tool = app.extensionRegistry->registerTool(definition);
    tools.push_back(tool);
}