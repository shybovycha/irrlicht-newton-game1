cc_library(
    name = "dAnimation",
    hdrs = glob([ "packages/dAnimation/*.h" ]),
    includes = [ "packages/dAnimation/" ],
    strip_include_prefix = "packages/dAnimation/",
    srcs = glob([ "packages/dAnimation/*.cpp" ]),
    deps = [
        ":dMath",
        ":dContainers",
    ],
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "dContainers",
    hdrs = glob([ "packages/dContainers/*.h" ]),
    includes = [ "packages/dContainers/" ],
    strip_include_prefix = "packages/dContainers/",
    srcs = glob([ "packages/dContainers/*.cpp" ]),
    deps = [
        ":dMath",
    ],
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "dCustomJoints",
    hdrs = glob([ "packages/dCustomJoints/*.h" ]),
    includes = [ "packages/dCustomJoints/" ],
    strip_include_prefix = "packages/dCustomJoints/",
    srcs = glob([ "packages/dCustomJoints/*.cpp" ]),
    deps = [
        ":dMath",
        ":dContainers",
        "//vendor/newton-dynamics/coreLibrary_300:NewtonStatic",
    ],
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "dMath",
    hdrs = glob([ "packages/dMath/*.h" ]),
    includes = [ "packages/dMath/" ],
    strip_include_prefix = "packages/dMath/",
    srcs = glob([ "packages/dMath/*.cpp" ]),
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "dScene",
    hdrs = glob([ "packages/dScene/*.h" ]),
    srcs = glob([ "packages/dScene/*.cpp" ]),
    deps = [
        ":dMath",
        ":dContainers",
        ":dAnimation",
        "//vendor/newton-dynamics/coreLibrary_300:NewtonStatic",
        "//vendor/tinyxml:tinyxml",
    ],
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "dVisualDebuggerServer",
    hdrs = glob([ "packages/dVisualDebuggerServer/*.h" ]),
    srcs = glob([ "packages/dVisualDebuggerServer/*.cpp" ]),
    deps = [
        ":dMath",
        ":dContainers",
        ":dAnimation",
        "//vendor/newton-dynamics/coreLibrary_300:NewtonStatic",
        "//vendor/enet:enet"
    ],
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "newton-dynamics-shared",
    deps = [
        "//vendor/newton-dynamics/coreLibrary_300:Newton",
        ":dContainers",
        ":dAnimation",
        ":dCustomJoints",
        ":dMath",
        ":dScene",
    ],
    visibility = [ "//visibility:public" ]
)

cc_library(
    name = "newton-dynamics",
    deps = [
        "//vendor/tinyxml:tinyxml",
        "//vendor/newton-dynamics/coreLibrary_300:NewtonStatic",
        ":dContainers",
        ":dAnimation",
        ":dCustomJoints",
        ":dMath",
        ":dScene",
    ],
    visibility = [ "//visibility:public" ]
)

