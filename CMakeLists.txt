cmake_minimum_required(VERSION 3.1)
project(irrlicht_newton_game1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
set(EXECUTABLE_NAME irrlicht_newton_game1)
set(IRRLICHT_PATH /home/shybovycha/irrlicht-1.8.3)

find_library(IRRLICHT_LIBRARY_PATH
        NAMES Irrlicht
        PATHS ${IRRLICHT_PATH}/lib/
        PATH_SUFFIXES Linux MacOSX Win32-gcc Win32-visualstudio Win64-visualstudio
        NO_DEFAULT_PATH)

find_package(X11)
find_package(OpenGL)
find_package(ZLIB)

message(STATUS "Found Irrlicht: ${IRRLICHT_LIBRARY_PATH}")

include_directories(${IRRLICHT_PATH}/include)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME}
        ${IRRLICHT_LIBRARY_PATH}
        ${X11_LIBRARIES}
        ${OPENGL_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${X11_Xxf86vm_LIB})
