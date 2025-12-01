cmake_minimum_required(VERSION 4.0)
project(Smile VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Lua static library
add_library(lua STATIC
    extern/lua/lapi.c
    extern/lua/lauxlib.c
    extern/lua/lbaselib.c
    extern/lua/lcode.c
    extern/lua/lcorolib.c
    extern/lua/lctype.c
    extern/lua/ldblib.c
    extern/lua/ldebug.c
    extern/lua/ldo.c
    extern/lua/ldump.c
    extern/lua/lfunc.c
    extern/lua/lgc.c
    extern/lua/linit.c
    extern/lua/liolib.c
    extern/lua/llex.c
    extern/lua/lmathlib.c
    extern/lua/lmem.c
    extern/lua/loadlib.c
    extern/lua/lobject.c
    extern/lua/lopcodes.c
    extern/lua/loslib.c
    extern/lua/lparser.c
    extern/lua/lstate.c
    extern/lua/lstring.c
    extern/lua/lstrlib.c
    extern/lua/ltable.c
    extern/lua/ltablib.c
    extern/lua/ltm.c
    extern/lua/lua.c
    extern/lua/luac.c
    extern/lua/lundump.c
    extern/lua/lutf8lib.c
    extern/lua/lvm.c
    extern/lua/lzio.c
)
target_include_directories(lua PUBLIC extern/lua)

# GLAD static library
add_library(glad STATIC
    extern/glad/src/gl.c
    extern/glad/src/wgl.c
)
target_include_directories(glad PUBLIC extern/glad/include)

# FreeType static library
add_subdirectory(extern/freetype)

# Smile executable
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(Smile ${SOURCES})

target_compile_options(Smile PRIVATE /bigobj)

# Add module interfaces
target_sources(Smile
    PRIVATE
        FILE_SET cxx_modules TYPE CXX_MODULES
        FILES
            src/type/numbers.cppm
)

if(WIN32)
    # target_sources(Smile PRIVATE win.rc) # Windows resources
    # set_target_properties(Smile PROPERTIES
    #     WIN32_EXECUTABLE TRUE
    # )
endif()

# Include directories (external)
target_include_directories(Smile PRIVATE extern/lua extern/glad/include extern/freetype/include extern src ${CMAKE_SOURCE_DIR})

# Link libraries
target_link_libraries(Smile PRIVATE lua glad opengl32 freetype)

if(WIN32)
    target_link_libraries(Smile PRIVATE dwmapi winmm)
    add_custom_command(TARGET Smile POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/extern/zlib1.dll"
            $<TARGET_FILE_DIR:Smile>
    )
endif()

# if (MINGW)
#     # bundle these so it works everywhere
#     target_link_options(Smile PRIVATE -static -static-libgcc -static-libstdc++)
#     # random crap to fix sol binding
#     target_compile_options(Smile PRIVATE 
#         -Wa,-mbig-obj
#         -ftemplate-backtrace-limit=0
#         -O1  # Lower optimization to reduce template bloat
#     )
#     target_link_options(Smile PRIVATE -Wl,--allow-multiple-definition)
# endif()

# Custom target to copy extensions, assets (and licenses) every build
add_custom_target(copy_extensions
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/extensions"
        "$<TARGET_FILE_DIR:Smile>/extensions"
)
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Smile>/assets"
)
add_custom_target(copy_licenses
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/third_party_licenses.txt"
        "$<TARGET_FILE_DIR:Smile>/third_party_licenses.txt"
)

# Make these run as part of ALL builds
add_custom_target(prebuild ALL
    DEPENDS copy_extensions copy_assets copy_licenses
)