cmake_minimum_required(VERSION 2.6)

project(LuaCppInterface)

find_package(lua REQUIRED)

include_directories(LUA_INCLUDE_DIR)

set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")

set(LUACPPINTERFACE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/LuaCppInterface)

set(LUACPPINTERFACE_SOURCES
    ${LUACPPINTERFACE_SOURCE_DIR}/luacoroutine.cpp
    ${LUACPPINTERFACE_SOURCE_DIR}/luareference.cpp
    ${LUACPPINTERFACE_SOURCE_DIR}/luacppinterface.cpp
    ${LUACPPINTERFACE_SOURCE_DIR}/luatable.cpp
    ${LUACPPINTERFACE_SOURCE_DIR}/luafunction.cpp
    ${LUACPPINTERFACE_SOURCE_DIR}/luaerror.cpp
    ${LUACPPINTERFACE_SOURCE_DIR}/luastringconversion.cpp
)

set(LUACPPINTERFACE_INCLUDE_FILES
    ${LUACPPINTERFACE_SOURCE_DIR}/luacoroutine.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luacppinclude.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luacppinterface.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luaerror.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luafunction.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luafunctionbase.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luageneralgenericfunctions.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luagenericfunction.h
    ${LUACPPINTERFACE_SOURCE_DIR}/lualightuserdata.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luareference.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luastringconversion.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luatable.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luatype.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luatypetemplates.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luauserdata.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luauserdatabindtemplates.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luauserdataforwards.h
    ${LUACPPINTERFACE_SOURCE_DIR}/luavoidgenericfunctions.h
)

set(LUACPPINTERFACE_LIBRARY luacppinterface)

add_library(${LUACPPINTERFACE_LIBRARY} SHARED ${LUACPPINTERFACE_SOURCES})

target_link_libraries(${LUACPPINTERFACE_LIBRARY} ${LUA_LIBRARIES})

install(FILES ${LUACPPINTERFACE_INCLUDE_FILES} DESTINATION include/luacppinterface)
install(TARGETS ${LUACPPINTERFACE_LIBRARY} DESTINATION lib)
install(FILES luacppinterface-config.cmake DESTINATION lib/cmake/luacppinterface)
