config_setting(
  name = "windows",
  constraint_values = [
    "@bazel_tools//platforms:windows",
  ],
)

cc_library(
    name = "luaconf",
    hdrs = [ "src/luaconf.h" ],
)

cc_library(
    name = "lua",
    hdrs = glob([ "src/*.h" ]),
    includes = [ "src" ],
    srcs = glob([ "src/*.c" ], exclude = [ "src/luac.c" ]),
    copts = select({
        ":windows": [],
        "//conditions:default": [
            "-std=gnu99",
            "-O2",
            "-Wall", # OSX/Linux only
            "-Wextra",
        ],
    }) + [
        "-DLUA_COMPAT_5_2"
    ],
    linkopts = [ "-lm" ],
    deps = [ ":luaconf" ],
    visibility = [ "//visibility:public" ],
)
