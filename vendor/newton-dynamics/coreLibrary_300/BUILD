config_setting(
  name = "windows",
  constraint_values = [
    "@bazel_tools//platforms:windows",
  ],
)

cc_library(
    name = "pthreads",
    hdrs = [
        "source/pthreads.2/pthread.h",
        "source/pthreads.2/implement.h",
        "source/pthreads.2/semaphore.h",
        "source/pthreads.2/sched.h",
    ],
    linkopts = [
        "-fdeclspec",
    ],
    srcs = [
        "source/pthreads.2/pthread.c",
    ],
)

cc_library(
    name = "coreLibrary_300",
    deps = select({
        ":windows": [
            ":pthreads",
        ],
        "//conditions:default": [],
    }),
    hdrs = glob([
        "source/core/*.h",
        "source/physics/*.h",
        "source/meshUtil/*.h",
        "source/newton/*.h"
    ]),
    includes = [
        "source/core/",
        "source/physics/",
        "source/meshUtil/",
        "source/newton/"
    ],
    srcs = glob([
        "source/core/*.cpp",
        "source/physics/*.cpp",
        "source/meshUtil/*.cpp",
        "source/newton/*.cpp"
    ]),
    copts = [
        "-fpic",
        "-msse",
        "-msse3",
        "-msse4",
        "-mfpmath=sse",
        "-ffloat-store",
        "-ffast-math",
        "-freciprocal-math",
        "-funsafe-math-optimizations",
        "-fsingle-precision-constant"
    ],
)

cc_library(
    name = "NewtonStatic",
    deps = [ ":coreLibrary_300" ],
    linkstatic = True,
    visibility = [ "//visibility:public" ],
)

cc_library(
    name = "Newton",
    deps = [ ":coreLibrary_300" ],
    visibility = [ "//visibility:public" ],
)
