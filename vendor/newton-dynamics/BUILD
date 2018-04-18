config_setting(
  name = "windows",
  constraint_values = [
    "@bazel_tools//platforms:windows",
  ],
)

config_setting(
  name = "osx",
  constraint_values = [
    "@bazel_tools//platforms:osx",
  ],
)

config_setting(
  name = "linux",
  constraint_values = [
    "@bazel_tools//platforms:linux",
  ],
)

newton_copts = [
    "-fpic",
    "-msse3",
    "-msse4",
    "-mfpmath=sse",
    "-ffloat-store",
    "-ffast-math",
    "-freciprocal-math",
    "-funsafe-math-optimizations",
    "-fsingle-precision-constant"
]

cc_library(
    name = "dAnimation",
    deps = [
        ":dg",
        ":dContainers",
    ],
    hdrs = glob([ "sdk/dAnimation/*.h" ]),
    includes = [ "sdk/dAnimation/" ],
    srcs = glob([ "sdk/dAnimation/*.cpp" ]),
)

cc_library(
    name = "dContainers",
    deps = [
        ":dg",
        ":dMath",
    ],
    hdrs = glob([ "sdk/dContainers/*.h" ]),
    includes = [ "sdk/dContainers/" ],
    srcs = glob([ "sdk/dContainers/*.cpp" ]),
)

cc_library(
    name = "dCustomJoints",
    deps = [
        ":dg",
        ":dContainers",
    ],
    hdrs = glob([ "sdk/dCustomJoints/*.h" ]),
    includes = [ "sdk/dCustomJoints/" ],
    srcs = glob([ "sdk/dCustomJoints/*.cpp" ]),
)

cc_library(
    name = "dMath",
    deps = [
        ":dg",
    ],
    hdrs = glob([ "sdk/dMath/*.h" ]),
    includes = [ "sdk/dMath/" ],
    srcs = glob([ "sdk/dMath/*.cpp" ]),
    copts = newton_copts,
)

cc_library(
    name = "dNewton",
    deps = [
        ":dg",
        ":dCustomJoints",
    ],
    hdrs = glob([ "sdk/dNewton/*.h" ]),
    includes = [ "sdk/dNewton/" ],
    srcs = glob([ "sdk/dNewton/*.cpp" ]),
)

cc_library(
    name = "dScene",
    deps = [
        ":dg",
        ":dMath",
        ":dAnimation",
        "//vendor/tinyxml:tinyxml",
    ],
    hdrs = glob([ "sdk/dScene/*.h" ]),
    includes = [ "sdk/dScene/" ],
    srcs = glob([ "sdk/dScene/*.cpp" ]),
)

cc_library(
    name = "dg",
    deps = [],
    hdrs = glob([
        "sdk/dgCore/*.h",
        "sdk/dgMeshUtil/*.h",
        "sdk/dgNewton/*.h",
        "sdk/dgPhysics/*.h",
    ]),
    includes = [
        "sdk/dgCore/",
        "sdk/dgMeshUtil/",
        "sdk/dgNewton/",
        "sdk/dgPhysics/",
    ],
    srcs = glob([
        "sdk/dgCore/*.cpp",
        "sdk/dgMeshUtil/*.cpp",
        "sdk/dgNewton/*.cpp",
        "sdk/dgPhysics/*.cpp",
    ]),
    copts = newton_copts,
    defines = select({
        ":osx": [
            "_MACOSX_VER",
        ],
        ":windows": [
            "_WIN_32_VER",
        ],
        ":linux": [
            "_POSIX_VER",
        ],
        "//conditions:default": [],
    }),
)

cc_library(
    name = "newton-dynamics",
    deps = [
        "//vendor/tinyxml:tinyxml",
        ":dContainers",
        ":dAnimation",
        ":dCustomJoints",
        ":dMath",
        ":dScene",
        ":dNewton",
        ":dg",
    ],
    copts = newton_copts,
    visibility = [ "//visibility:public" ]
)
