cmake_minimum_required(VERSION 2.6)
project(irrlicht_newton_game1)

set(CMAKE_CXX_FLAGS "-std=c++14")

option("NEWTON_DEMOS_SANDBOX" "Build demos sandbox" OFF)

set(ENet_PATH ${PROJECT_SOURCE_DIR}/source/enet-master)
set(IRRLICHT_PATH ${PROJECT_SOURCE_DIR}/source/irrlicht-1.8.3)

# set(LUACPPINTERFACE_PATH ${PROJECT_SOURCE_DIR}/source/luacppinterface-master)
# set(CPPFORMAT_PATH ${PROJECT_SOURCE_DIR}/source/cppformat-master)

set(NEWTONGD_PATH ${PROJECT_SOURCE_DIR}/source/newton-dynamics-master)

set(NEWTONGD_INCLUDE_DIRS
        ${NEWTONGD_PATH}/packages/dCustomJoints
        ${NEWTONGD_PATH}/packages/dContainers
        ${NEWTONGD_PATH}/packages/dMath)

set(ENet_INCLUDE_DIRS ${ENet_PATH}/include)
# set(LUACPPINTERFACE_INCLUDE_DIR ${LUACPPINTERFACE_PATH}/LuaCppInterface)
# set(LUA_INCLUDE_DIR ${LUACPPINTERFACE_PATH}/lua/src)

set(NEWTON_LIBRARIES Newton dMath)
set(ENet_LIBRARY enet)

# set(LUA_LIBRARIES lua)
# set(LUACPPINTERFACE_LIBRARIES luacppinterface)

add_subdirectory(${ENet_PATH})
# add_subdirectory(${LUACPPINTERFACE_PATH})
# add_subdirectory(${CPPFORMAT_PATH})
add_subdirectory(${NEWTONGD_PATH})

set(luacppinterface_DIR ${PROJECT_SOURCE_DIR}/luacppinterface-master)
set(lua_DIR ${PROJECT_SOURCE_DIR}/lua)

find_package(OpenGL)
find_package(ZLIB)
find_package(lua)
find_package(luacppinterface)
# find_package(irrlicht)

if (NOT IRRLICHT_LIBRARY_PATH)
    if (UNIX)
        set(IRRLICHT_PATH_SUFFIX Linux)
    endif()

    if (APPLE)
        set(IRRLICHT_PATH_SUFFIX MacOSX)
    endif()

    if (WIN32)
        if (MSVC)
            set(IRRLICHT_PATH_SUFFIX Win32-visualstudio Win64-visualstudio)
        endif()

        if (MINGW)
            set(IRRLICHT_PATH_SUFFIX Win32-gcc)
        endif()
    endif()

    find_library(IRRLICHT_LIBRARY_PATH
            NAMES Irrlicht
            PATHS ${IRRLICHT_PATH}/lib/
            PATH_SUFFIXES ${IRRLICHT_PATH_SUFFIX})

    message(STATUS "Found Irrlicht: ${IRRLICHT_LIBRARY_PATH}")
endif()

set(IRRLICHT_INCLUDE_DIR ${IRRLICHT_PATH}/include)

set(LUACPPINTERFACE_LIBRARY luacppinterface)

set(LIBRARIES
    ${LUACPPINTERFACE_LIBRARY_PATH}
    ${NEWTON_LIBRARIES}
    ${IRRLICHT_LIBRARY_PATH}
    ${OPENGL_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${LUA_LIBRARY}
)

if (UNIX)
    if (NOT APPLE)
        find_package(X11)
        set(LIBRARIES ${LIBRARIES} ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
    endif()
endif()

include_directories(
    ${IRRLICHT_INCLUDE_DIR}
    ${LUACPPINTERFACE_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIR}
    ${NEWTONGD_INCLUDE_DIRS}
)

set(SOURCE_FILES
    source/main.cpp
)

set(EXECUTABLE_NAME irrlicht_newton_game1)

if (APPLE)
    find_library(IOKIT NAMES IOKit)
    find_library(OpenGL NAMES OpenGL)
    find_library(COREVIDEO NAMES CoreVideo)
    find_library(COCOA NAMES Cocoa)
    set(LIBRARIES ${LIBRARIES} ${IOKIT} ${COREVIDEO} ${COCOA})
endif()

if (APPLE)
    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${SOURCE_FILES})
else()
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
endif()

target_link_libraries(${EXECUTABLE_NAME} ${LIBRARIES})
