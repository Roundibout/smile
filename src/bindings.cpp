#include <sol/sol.hpp>

#include <datatypes/vector2.hpp>
#include <datatypes/rect.hpp>
#include <datatypes/color4.hpp>
#include <datatypes/ui_types.hpp>

#include <window/window.hpp>
#include <window/window_input.hpp>
#include <ui/theme.hpp>
#include <app/app.hpp>

void register_bindings(sol::state& lua) {
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
        "SetCorners", &UILayout::setCorners
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
        "scroll", sol::property(
            [](MouseInput& self) -> Vector2& {return self.scroll;},
            [](MouseInput& self, Vector2 scroll) {self.scroll = scroll;}
        ) // same thing
    );

    lua.new_usertype<WindowInput>("WindowInput",
        "type", &WindowInput::type,
        "mouse", sol::property([](WindowInput& self) -> MouseInput& {return self.mouse;}), // nested struct
        "key", &WindowInput::key
    );

    // -- Classes -- //

    lua.new_usertype<Renderer>("Renderer",
        "Dirty", &Renderer::dirty,
        "IsDirty", &Renderer::isDirty,

        "ResolvePosition", &Renderer::resolvePosition,
        "ResolveLayout", &Renderer::resolveLayout,
        "ApplyLayout", &Renderer::applyLayout,

        "DrawTriangle", &Renderer::drawTriangle,
        "DrawQuad", &Renderer::drawQuad,
        "DrawRect", &Renderer::drawRect,
        "DrawStrokeRect", &Renderer::drawStrokeRect,
        "DrawRoundedRect", &Renderer::drawRoundedRect,
        "DrawRoundedRect", &Renderer::drawRoundedStrokeRect,
        "DrawText", &Renderer::drawText
    );

    lua.new_usertype<Window>("Window",
        "renderer", sol::property([](Window& self) -> Renderer& {return self.renderer;}), // nested struct

        "ConnectUpdate", [](Window& self, sol::function callback){self.connectCallback(WindowEvent::Update, callback);},
        "ConnectRender", [](Window& self, sol::function callback){self.connectCallback(WindowEvent::Render, callback);},
        "ConnectInput", [](Window& self, sol::function callback){self.connectCallback(WindowEvent::Input, callback);}
    );

    // -- Singletons -- //

    lua.new_usertype<Theme>("Theme",
        "GetMetric", &Theme::getMetric,
        "GetColor", &Theme::getColor,
        "GetFont", &Theme::getFont
    );

    lua.new_usertype<App>("App",
        "Run", &App::run,
        "Quit", &App::quit,

        "CreateWindow", &App::createWindow,

        "SetUIScale", &App::setUIScale
    );

    // Assign the singletons to global variables so you don't need the get function
    lua["Theme"] = &Theme::get();
    lua["App"] = &App::get();
}