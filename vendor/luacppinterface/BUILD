cc_library(
    name = "luacppinterface",
    deps = [
        "//vendor/lua:lua"
    ],
    hdrs = glob([ "LuaCppInterface/*.h" ]),
    includes = [ "LuaCppInterface" ],
    strip_include_prefix = "LuaCppInterface/",
    srcs = [
        "LuaCppInterface/luacoroutine.cpp",
        "LuaCppInterface/luacppinterface.cpp",
        "LuaCppInterface/luafunction.cpp",
        "LuaCppInterface/luareference.cpp",
        "LuaCppInterface/luatable.cpp",
        "LuaCppInterface/luaerror.cpp",
        "LuaCppInterface/luastringconversion.cpp",
    ],
    copts = [
        "-std=c++11",
        "-stdlib=libc++",
        "-xc++",
    ],
    visibility = [ "//visibility:public" ],
)
