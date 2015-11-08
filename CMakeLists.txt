cmake_minimum_required(VERSION 3.1)
project(irrlicht_newton_game1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option("NEWTON_DEMOS_SANDBOX" "Build demos sandbox" OFF)

set(LUACPPINTERFACE_PATH source/luacppinterface-master)
set(CPPFORMAT_PATH source/cppformat-master)
set(NEWTONGD_PATH source/newton-dynamics-master)
set(NEWTONGD_INCLUDE_DIRS
        ${NEWTONGD_PATH}/packages/dCustomJoints
        ${NEWTONGD_PATH}/packages/dContainers
        ${NEWTONGD_PATH}/packages/dMath
        )

set(NEWTON_LIBRARIES Newton dMath)

add_subdirectory(${LUACPPINTERFACE_PATH})
add_subdirectory(${CPPFORMAT_PATH})
add_subdirectory(${NEWTONGD_PATH})

find_package(X11)
find_package(OpenGL)
find_package(ZLIB)
find_package(Lua)

if (NOT IRRLICHT_LIBRARY_PATH)
    find_library(IRRLICHT_LIBRARY_PATH
            NAMES Irrlicht
            PATHS ${IRRLICHT_PATH}/lib/
            PATH_SUFFIXES Linux MacOSX Win32-gcc Win32-visualstudio Win64-visualstudio)

    message(STATUS "Found Irrlicht: ${IRRLICHT_LIBRARY_PATH}")
endif()

include_directories(${IRRLICHT_PATH}/include
        ${LUA_INCLUDE_DIR}
        ${LUACPPINTERFACE_PATH}/include
        ${CPPFORMAT_PATH}
        ${NEWTONGD_INCLUDE_DIRS})

set(SOURCE_FILES source/main.cpp)
set(EXECUTABLE_NAME irrlicht_newton_game1)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME}
        luacppinterface
        cppformat
        ${NEWTON_LIBRARIES}
        ${IRRLICHT_LIBRARY_PATH}
        ${X11_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${X11_Xxf86vm_LIB}
        ${LUA_LIBRARIES})
