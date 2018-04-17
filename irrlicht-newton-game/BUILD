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
  data = glob([ "media/**" ], exclude = [ "media/BUILD", "media/BUCK", "media/.keep" ]),
  deps = [
    "//vendor/irrlicht:irrlicht",
    "//vendor/luacppinterface:luacppinterface",
    "//vendor/newton-dynamics:newton-dynamics",
  ],
  copts = [
    "-std=c++17",
  ],
  linkstatic = True,
)
