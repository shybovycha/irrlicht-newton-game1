# Newton GD for Irrlicht game developers

## Briefing

This repo contains the code for Irrlicht + Newton game tutorial. It starts from building 
a sample Irrlicht program, then goes through the whole game development process, including
creating models and embedding scripts.

## Changelog

**8 Nov 2015** Lua scripting is added to the program and all the interaction logic is moved there.

**7 Nov 2015** Sample Irrlicht program is compiled. Project is made using CMake build system.

## Step-by-step

1. download Irrlicht 1.8.3
2. unzip Irrlicht & go to its' `/source/Irrlicht` subdirectory in terminal
3. run `make`
4. install `lua-dev` and `cmake` packages for your system
5. go to the project directory, create the `build` sub-dir, navigate there and run 

        cmake .. -DIRRLICHT_PATH=PATH_TO_IRRLICHT_GOES_HERE 
        make
        cp -r ../media/ . 
        ./irrlicht_newton_game1 

    replacing `PATH_TO_IRRLICHT_GOES_HERE` with the base directory of your Irrlicht SDK.

## Important hints

### CMake for Irrlicht

You will need to use `find_library()` to look for Irrlicht libraries:

    find_library(IRRLICHT_LIBRARY_PATH
            NAMES Irrlicht
            PATHS ${IRRLICHT_PATH}/lib/
            PATH_SUFFIXES Linux MacOSX Win32-gcc Win32-visualstudio Win64-visualstudio)

Here I used the `PATH_SUFFIXES` to minimize duplication when using multiple `PATHS` of the same prefix.

Also, I made variable `IRRLICHT_PATH` not to be set in the `CMakeLists`, so you'll need to specify your
path to Irrlicht base directory when running CMake: `cmake -DIRRLICHT_PATH=/Users/my-username/irrlicht-1.8.3`
(or whatever you have).

Also, you will need these libraries to be found with `find_package()`: `X11`, `OpenGL`, `ZLIB`
(all are case-sensitive).

Then you'll need to set include directories with `include_directories(${IRRLICHT_PATH}/include)`.
This must be done BEFORE calling `add_executable()`.

And only AFTER calling `add_executable()` you'll need to set libraries with

    target_link_libraries(${EXECUTABLE_NAME}
            ${IRRLICHT_LIBRARY_PATH}
            ${X11_LIBRARIES}
            ${OPENGL_LIBRARIES}
            ${ZLIB_LIBRARIES}
            ${X11_Xxf86vm_LIB})

Note: sequence matters here! So, if you set Irrlicht libraries to be the last entry here,
you will fall into lots of `undefined reference to glPixelStorei()` (and many other) non-obvious errors.

### Lua importing

    extern "C" {
        #include <lua.h>
        #include <lauxlib.h>
    };

Those two little bastards MUST be wrapped with `extern "C"`, otherwise you will get
`undefined reference to luaL_newstate()`-like errors.

### CMake subprojects

When you are having a sub-project, say, `luacppinterface` library, your main project depends on, you may want to
build everything with a single CMake run. To make it happen, all you need is only these two lines:
 
    add_subdirectory(${LUACPPINTERFACE_PATH})
    
    # ...
    
    target_link_libraries(${EXECUTABLE_NAME} luacppinterface)