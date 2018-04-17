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

genrule(
  name = "media",
  srcs = glob([
    "media/**",
  ]),
  outs = [],
  cmd = "cp -r $SRCS $(@D)",
  output_to_bindir = True,
)

cc_binary(
  name = "irrlicht-newton-game",
  srcs = [
    "main.cpp",
  ],
  # precompiled_header = ":stdafx",
  data = glob([ "media/**" ], exclude = [ "media/BUILD", "media/BUCK", "media/.keep" ]),
  deps = [
    "//vendor/irrlicht:irrlicht",
    # ":media",
    "//vendor/luacppinterface:luacppinterface",
    "//vendor/newton-dynamics:newton-dynamics",
    # "//vendor/tinyxml:tinyxml",
    # "//media:models",
    # "//media:scripts",
    # "//media:textures",
  ],
  copts = [
    "-std=c++17",
  ],
  linkstatic = True,
  linkopts = select({
    ":osx": [
      "-lGL",
      "-lstdc++",
      "-lXxf86vm",
      "-framework Foundation",
      "-framework IOKit",
      "-framework CoreVideo",
      "-framework Cocoa",
      "-framework OpenGL"
    ],
    "//conditions:default": []
  }),
  # platform_linker_flags = [
  #  (".*osx.*", [
  #     "-lGL",
  #     "-lstdc++",
  #     "-lXxf86vm",
  #     "-framework Foundation",
  #     "-framework IOKit",
  #     "-framework CoreVideo",
  #     "-framework Cocoa",
  #     "-framework OpenGL"
  #   ])
  # ],
)
