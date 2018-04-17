# config_setting(
#   name = "windows",
#   constraint_values = [
#     "@bazel_tools//platforms:windows",
#   ],
# )

# cc_library(
#     name = "pthreads",
#     hdrs = [
#         "source/pthreads.2/pthread.h",
#         "source/pthreads.2/implement.h",
#         "source/pthreads.2/semaphore.h",
#         "source/pthreads.2/sched.h",
#     ],
#     linkopts = [
#         "-fdeclspec",
#     ],
#     srcs = [
#         "source/pthreads.2/pthread.c",
#     ],
# )

# cc_library(
#     name = "core",
#     includes = [ "source/core/" ],
#     strip_include_prefix = "source/core/",
#     hdrs = glob([ "source/core/*.h" ]),
#     srcs = glob([ "source/core/*.cpp" ]),
#     copts = [
#         "-fpic",
#         "-msse",
#         "-msse3",
#         "-msse4",
#         "-mfpmath=sse",
#         "-ffloat-store",
#         "-ffast-math",
#         "-freciprocal-math",
#         "-funsafe-math-optimizations",
#         "-fsingle-precision-constant"
#     ],
# )

# cc_library(
#     name = "physics",
#     deps = [
#         ":core",
#     ],
#     includes = [ "source/physics/" ],
#     strip_include_prefix = "source/physics/",
#     hdrs = glob([ "source/physics/*.h" ]),
#     srcs = glob([ "source/physics/*.cpp" ]),
#     copts = [
#         "-fpic",
#         "-msse",
#         "-msse3",
#         "-msse4",
#         "-mfpmath=sse",
#         "-ffloat-store",
#         "-ffast-math",
#         "-freciprocal-math",
#         "-funsafe-math-optimizations",
#         "-fsingle-precision-constant"
#     ],
# )

# cc_library(
#     name = "meshUtil",
#     deps = [
#         ":physics",
#     ],
#     includes = [ "source/meshUtil/" ],
#     strip_include_prefix = "source/meshUtil/",
#     hdrs = glob([ "source/meshUtil/*.h" ]),
#     srcs = glob([ "source/meshUtil/*.cpp" ]),
#     copts = [
#         "-fpic",
#         "-msse",
#         "-msse3",
#         "-msse4",
#         "-mfpmath=sse",
#         "-ffloat-store",
#         "-ffast-math",
#         "-freciprocal-math",
#         "-funsafe-math-optimizations",
#         "-fsingle-precision-constant"
#     ],
# )

# cc_library(
#     name = "newton",
#     deps = [
#         ":core",
#         ":physics",
#     ],
#     includes = [ "source/newton/" ],
#     strip_include_prefix = "source/newton/",
#     hdrs = glob([ "source/newton/*.h" ]),
#     srcs = glob([ "source/newton/*.cpp" ]),
#     copts = [
#         "-fpic",
#         "-msse",
#         "-msse3",
#         "-msse4",
#         "-mfpmath=sse",
#         "-ffloat-store",
#         "-ffast-math",
#         "-freciprocal-math",
#         "-funsafe-math-optimizations",
#         "-fsingle-precision-constant"
#     ],
# )

# cc_library(
#     name = "coreLibrary_300",
#     deps = select({
#         ":windows": [
#             ":pthreads",
#         ],
#         "//conditions:default": [],
#     }) + [
#         ":core",
#         ":physics",
#         ":meshUtil",
#         ":newton"
#     ]
# )

# cc_library(
#     name = "NewtonStatic",
#     deps = [ ":coreLibrary_300" ],
#     linkstatic = True,
#     visibility = [ "//visibility:public" ],
# )

# cc_library(
#     name = "Newton",
#     deps = [ ":coreLibrary_300" ],
#     visibility = [ "//visibility:public" ],
# )


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
