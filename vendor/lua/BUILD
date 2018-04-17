cc_library(
    name = "luaconf",
    hdrs = [ "src/luaconf.h" ],
)

cc_library(
    name = "lua",
    hdrs = glob([ "src/*.h" ]),
    includes = [ "src" ],
    srcs = glob([ "src/*.c" ], exclude = [ "src/luac.c" ]),
    copts = [
        "-std=gnu99",
        "-O2",
        "-Wall",
        "-Wextra",
        "-DLUA_COMPAT_5_2"
    ],
    linkopts = [ "-lm" ],
    deps = [ ":luaconf" ],
    visibility = [ "//visibility:public" ],
)
