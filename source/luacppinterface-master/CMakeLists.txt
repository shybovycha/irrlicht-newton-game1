cmake_minimum_required (VERSION 2.6)

project(luacppinterface)

include_directories("lua/src")

# list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

find_package(Lua)

include_directories(${LUA_INCLUDE_DIR})

add_library(luacppinterface STATIC
    include/luacoroutine.cpp
    include/luareference.cpp
    include/luacppinterface.cpp
    include/luatable.cpp
    include/luafunction.cpp
)

target_link_libraries(luacppinterface ${LUA_LIBRARIES})