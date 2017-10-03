cmake_minimum_required(VERSION 2.6)

project(lua)

include_directories(src)

set(LUA_LIBRARY lua)

if(UNIX)
    set(SYSCFLAGS ${SYSCFLAGS} "-DLUA_USE_LINUX")
    set(SYSLIBS ${SYSLIBS} "-Wl,-E -ldl -lreadline")
endif()

if(APPLE)
    set(SYSCFLAGS ${SYSCFLAGS} "-DLUA_USE_MACOSX")
    set(SYSLIBS ${SYSLIBS} "-lreadline")
endif()

set(LUA_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
set(LUA_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(LUA_INCLUDE_FILES
    ${LUA_INCLUDE_DIR}/lprefix.h
    ${LUA_INCLUDE_DIR}/lua.h
    ${LUA_INCLUDE_DIR}/luaconf.h
    ${LUA_INCLUDE_DIR}/lapi.h
    ${LUA_INCLUDE_DIR}/llimits.h
    ${LUA_INCLUDE_DIR}/lstate.h
    ${LUA_INCLUDE_DIR}/lobject.h
    ${LUA_INCLUDE_DIR}/ltm.h
    ${LUA_INCLUDE_DIR}/lzio.h
    ${LUA_INCLUDE_DIR}/lmem.h
    ${LUA_INCLUDE_DIR}/ldebug.h
    ${LUA_INCLUDE_DIR}/ldo.h
    ${LUA_INCLUDE_DIR}/lfunc.h
    ${LUA_INCLUDE_DIR}/lgc.h
    ${LUA_INCLUDE_DIR}/lstring.h
    ${LUA_INCLUDE_DIR}/ltable.h
    ${LUA_INCLUDE_DIR}/lundump.h
    ${LUA_INCLUDE_DIR}/lvm.h
    ${LUA_INCLUDE_DIR}/lauxlib.h
    ${LUA_INCLUDE_DIR}/lualib.h
    ${LUA_INCLUDE_DIR}/lcode.h
    ${LUA_INCLUDE_DIR}/llex.h
    ${LUA_INCLUDE_DIR}/lopcodes.h
    ${LUA_INCLUDE_DIR}/lparser.h
    ${LUA_INCLUDE_DIR}/lctype.h
)

list(REMOVE_DUPLICATES LUA_INCLUDE_FILES)

set(LUA_SOURCE_FILES
    ${LUA_SOURCE_DIR}/lapi.c
    ${LUA_SOURCE_DIR}/lauxlib.c
    ${LUA_SOURCE_DIR}/lbaselib.c
    ${LUA_SOURCE_DIR}/lbitlib.c
    ${LUA_SOURCE_DIR}/lcode.c
    ${LUA_SOURCE_DIR}/lcorolib.c
    ${LUA_SOURCE_DIR}/lctype.c
    ${LUA_SOURCE_DIR}/ldblib.c
    ${LUA_SOURCE_DIR}/ldebug.c
    ${LUA_SOURCE_DIR}/ldo.c
    ${LUA_SOURCE_DIR}/ldump.c
    ${LUA_SOURCE_DIR}/lfunc.c
    ${LUA_SOURCE_DIR}/lgc.c
    ${LUA_SOURCE_DIR}/linit.c
    ${LUA_SOURCE_DIR}/liolib.c
    ${LUA_SOURCE_DIR}/llex.c
    ${LUA_SOURCE_DIR}/lmathlib.c
    ${LUA_SOURCE_DIR}/lmem.c
    ${LUA_SOURCE_DIR}/loadlib.c
    ${LUA_SOURCE_DIR}/lobject.c
    ${LUA_SOURCE_DIR}/lopcodes.c
    ${LUA_SOURCE_DIR}/loslib.c
    ${LUA_SOURCE_DIR}/lparser.c
    ${LUA_SOURCE_DIR}/lstate.c
    ${LUA_SOURCE_DIR}/lstring.c
    ${LUA_SOURCE_DIR}/lstrlib.c
    ${LUA_SOURCE_DIR}/ltable.c
    ${LUA_SOURCE_DIR}/ltablib.c
    ${LUA_SOURCE_DIR}/ltm.c
    ${LUA_SOURCE_DIR}/lua.c
    ${LUA_SOURCE_DIR}/lundump.c
    ${LUA_SOURCE_DIR}/lutf8lib.c
    ${LUA_SOURCE_DIR}/lvm.c
    ${LUA_SOURCE_DIR}/lzio.c
)

set(CMAKE_CXX_FLAGS "${SYSCFLAGS} -std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_2")
set(CMAKE_MODULE_LINKER_FLAGS "${SYSLIBS} -lm")

add_library(${LUA_LIBRARY} SHARED ${LUA_SOURCE_FILES} ${LUA_INCLUDE_FILES})

install(FILES ${LUA_INCLUDE_FILES} DESTINATION include/lua)
install(TARGETS ${LUA_LIBRARY} DESTINATION lib)
install(FILES lua-config.cmake DESTINATION lib/cmake/lua)
