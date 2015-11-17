cmake_minimum_required(VERSION 3.1)
project(irrlicht_newton_game1)

# if (APPLE)
#     set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-framework Foundation -framework OpenGL -framework Cocoa -framework Carbon -framework AppKit -framework IOKit")
# endif()

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

set(LIBRARIES luacppinterface
        cppformat
        ${NEWTON_LIBRARIES}
        ${IRRLICHT_LIBRARY_PATH}
        ${X11_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${LUA_LIBRARIES})

if (NOT APPLE)
    set(LIBRARIES ${LIBRARIES} ${X11_Xxf86vm_LIB})
endif()

include_directories(${IRRLICHT_PATH}/include
        ${LUA_INCLUDE_DIR}
        ${LUACPPINTERFACE_PATH}/include
        ${CPPFORMAT_PATH}
        ${NEWTONGD_INCLUDE_DIRS})

set(SOURCE_FILES source/main.cpp)
set(EXECUTABLE_NAME irrlicht_newton_game1)

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Foundation -framework OpenGL -framework Cocoa -framework Carbon -framework AppKit -framework IOKit")
endif()

if (APPLE)
    add_executable(${EXECUTABLE_NAME} MACOSX_BUNDLE ${SOURCE_FILES})
else()
    add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
endif()

target_link_libraries(${EXECUTABLE_NAME}
        ${LIBRARIES})