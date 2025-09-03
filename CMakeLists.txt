cmake_minimum_required(VERSION 3.10)
project(Smile VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Smile executable
add_executable(Smile src/main.cpp)

# Include directories (Lua and sol)
target_include_directories(Smile PRIVATE extern/lua extern)

# Link Lua
target_link_libraries(Smile PRIVATE lua)

# Copy scripts to build directory
add_custom_command(TARGET Smile POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/scripts"
    "$<TARGET_FILE_DIR:Smile>/scripts"
)

# Copy third-party licenses
add_custom_command(TARGET Smile POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/third_party_licenses.txt
    $<TARGET_FILE_DIR:Smile>/third_party_licenses.txt
)