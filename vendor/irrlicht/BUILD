load("@build_bazel_rules_apple//apple:ios.bzl", "ios_application")

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

cc_library(
  name = "bzip2",
  hdrs = glob([ "source/Irrlicht/bzip2/*.h" ]),
  includes = [ "include" ],
  textual_hdrs = glob([ "source/Irrlicht/bzip2/*.h" ]),
  srcs = [
    "source/Irrlicht/bzip2/blocksort.c",
    "source/Irrlicht/bzip2/huffman.c",
    "source/Irrlicht/bzip2/crctable.c",
    "source/Irrlicht/bzip2/randtable.c",
    "source/Irrlicht/bzip2/bzcompress.c",
    "source/Irrlicht/bzip2/decompress.c",
    "source/Irrlicht/bzip2/bzlib.c",
  ],
  copts = [
   "-xc"
  ],
)

cc_library(
  name = "jpeglib",
  hdrs = glob([ "source/Irrlicht/jpeglib/*.h" ]),
  textual_hdrs = glob([ "source/Irrlicht/jpeglib/*.h" ]),
  srcs = [
    "source/Irrlicht/jpeglib/jaricom.c",
    "source/Irrlicht/jpeglib/jcapimin.c",
    "source/Irrlicht/jpeglib/jcapistd.c",
    "source/Irrlicht/jpeglib/jcarith.c",
    "source/Irrlicht/jpeglib/jccoefct.c",
    "source/Irrlicht/jpeglib/jccolor.c",
    "source/Irrlicht/jpeglib/jcdctmgr.c",
    "source/Irrlicht/jpeglib/jchuff.c",
    "source/Irrlicht/jpeglib/jcinit.c",
    "source/Irrlicht/jpeglib/jcmainct.c",
    "source/Irrlicht/jpeglib/jcmarker.c",
    "source/Irrlicht/jpeglib/jcmaster.c",
    "source/Irrlicht/jpeglib/jcomapi.c",
    "source/Irrlicht/jpeglib/jcparam.c",
    "source/Irrlicht/jpeglib/jcprepct.c",
    "source/Irrlicht/jpeglib/jcsample.c",
    "source/Irrlicht/jpeglib/jctrans.c",
    "source/Irrlicht/jpeglib/jdapimin.c",
    "source/Irrlicht/jpeglib/jdapistd.c",
    "source/Irrlicht/jpeglib/jdarith.c",
    "source/Irrlicht/jpeglib/jdatadst.c",
    "source/Irrlicht/jpeglib/jdatasrc.c",
    "source/Irrlicht/jpeglib/jdcoefct.c",
    "source/Irrlicht/jpeglib/jdcolor.c",
    "source/Irrlicht/jpeglib/jddctmgr.c",
    "source/Irrlicht/jpeglib/jdhuff.c",
    "source/Irrlicht/jpeglib/jdinput.c",
    "source/Irrlicht/jpeglib/jdmainct.c",
    "source/Irrlicht/jpeglib/jdmarker.c",
    "source/Irrlicht/jpeglib/jdmaster.c",
    "source/Irrlicht/jpeglib/jdmerge.c",
    "source/Irrlicht/jpeglib/jdpostct.c",
    "source/Irrlicht/jpeglib/jdsample.c",
    "source/Irrlicht/jpeglib/jdtrans.c",
    "source/Irrlicht/jpeglib/jerror.c",
    "source/Irrlicht/jpeglib/jfdctflt.c",
    "source/Irrlicht/jpeglib/jfdctfst.c",
    "source/Irrlicht/jpeglib/jfdctint.c",
    "source/Irrlicht/jpeglib/jidctflt.c",
    "source/Irrlicht/jpeglib/jidctfst.c",
    "source/Irrlicht/jpeglib/jidctint.c",
    "source/Irrlicht/jpeglib/jquant1.c",
    "source/Irrlicht/jpeglib/jquant2.c",
    "source/Irrlicht/jpeglib/jutils.c",
    "source/Irrlicht/jpeglib/jmemmgr.c",
    "source/Irrlicht/jpeglib/jmemnobs.c",
  ],
  copts = [
    "-xc"
  ],
)

cc_library(
  name = "libpng",
  deps = [
    ":zlib",
  ],
  hdrs = glob([ "source/Irrlicht/libpng/*.h" ]),
  textual_hdrs = glob([ "source/Irrlicht/libpng/*.h" ]),
  srcs = glob([ "source/Irrlicht/libpng/*.c" ]),
  copts = [
    "-xc"
  ]
)

cc_library(
  name = "lzma",
  hdrs = glob([ "source/Irrlicht/lzma/*.h" ]),
  textual_hdrs = glob([ "source/Irrlicht/lzma/*.h" ]),
  srcs = glob([ "source/Irrlicht/lzma/*.c" ]),
  copts = [
    "-xc"
  ]
)

cc_library(
  name = "zlib",
  hdrs = [
    "source/Irrlicht/zlib/crc32.h",
    "source/Irrlicht/zlib/deflate.h",
    "source/Irrlicht/zlib/gzguts.h",
    "source/Irrlicht/zlib/inffast.h",
    "source/Irrlicht/zlib/inffixed.h",
    "source/Irrlicht/zlib/inflate.h",
    "source/Irrlicht/zlib/inftrees.h",
    "source/Irrlicht/zlib/trees.h",
    "source/Irrlicht/zlib/zutil.h",
    "source/Irrlicht/zlib/zlib.h",
    "source/Irrlicht/zlib/zconf.h",
  ],
  textual_hdrs = glob([ "source/Irrlicht/zlib/*.h" ]),
  strip_include_prefix = "source/Irrlicht/zlib/",
  srcs = glob([ "source/Irrlicht/zlib/*.c" ]),
  copts = [
    "-xc"
  ]
)

cc_library(
  name = "aesGladman",
  hdrs = glob([
    "include/*.h",
    "source/Irrlicht/aesGladman/*.h",
  ]),
  includes = [ "include" ],
  textual_hdrs = glob([
    "source/Irrlicht/*.h",
    "source/Irrlicht/aesGladman/*.h",
  ]),
  srcs = glob([ "source/Irrlicht/aesGladman/*.cpp" ]),
  copts = select({
    ":windows": [],
    "//conditions:default": [
      "-xc++",
      "-Wno-narrowing",
    ],
  }),
)

cc_import(
  name = "d3dx9",
  # hdrs = glob([ "$DXSDK_DIR/Include" ]),
  # static_library = "libmylib.lib", # A normal static library
  interface_library = "$(DXSDK_DIR)/Lib/x64/d3dx9.lib", # An import library for mylib.dll
  # shared_library = "mylib.dll",
  system_provided = True,
)

cc_library(
  name = "DirectX",
  deps = [
    ":d3dx9",
  ],
  includes = [
    "$(DXSDK_DIR)/Include",
  ],
)

objc_library(
  name = "irrlicht-osx",
  hdrs = glob([
    "include/*.h",
    "source/Irrlicht/*.h",
    "source/Irrlicht/MacOSX/*.h",
  ]),
  includes = [
    "include",
    "source/Irrlicht",
  ],
  textual_hdrs = glob([
    "include/*.h",
  ]),
  srcs = glob([
    "source/Irrlicht/*.cpp",
    "source/Irrlicht/MacOSX/*.mm",
  ]),
  deps = [
    ":libpng",
    ":jpeglib",
    ":lzma",
    ":bzip2",
    ":zlib",
    ":aesGladman",
  ],
  sdk_frameworks = [
    "Foundation",
    "IOKit",
    "OpenGL",
    "Cocoa",
    "CoreVideo",
  ],
  copts = [
    "-std=gnu++11",
    "-xobjective-c++",
    "-DIRRLICHT_EXPORTS=1",
    "-Wall",
    "-pipe",
    "-fno-objc-arc",
    "-fno-exceptions",
    "-fno-rtti",
    "-fstrict-aliasing",
    "-O3",
    "-DPNG_THREAD_UNSAFE_OK",
    "-DPNG_NO_MMX_CODE",
    "-DPNG_NO_MNG_FEATURES",
    "-fPIC",
  ],
  visibility = [
    "//visibility:public"
  ],
)

cc_library(
  name = "irrlicht-linux",
  hdrs = glob([
    "include/*.h",
    "source/Irrlicht/*.h",
  ]),
  textual_hdrs = glob([
    "include/*.h",
  ]),
  strip_include_prefix = "vendor/irrlicht/",
  srcs = glob([
    "source/Irrlicht/*.cpp",
  ]),
  deps = [
    ":libpng",
    ":jpeglib",
    ":lzma",
    ":bzip2",
    ":zlib",
    ":aesGladman",
  ],
  copts = [
    "-DIRRLICHT_EXPORTS=1",
    "-Wall",
    "-pipe",
    "-fno-exceptions",
    "-fno-rtti",
    "-fstrict-aliasing",
    "-O3",
    "-DPNG_THREAD_UNSAFE_OK",
    "-DPNG_NO_MMX_CODE",
    "-DPNG_NO_MNG_FEATURES",
    "-fPIC",
  ],
  visibility = [
    "//visibility:public"
  ],
)

cc_library(
  name = "irrlicht-windows",
  hdrs = glob([
    "include/*.h",
    "source/Irrlicht/*.h",
  ]),
  textual_hdrs = glob([
    "include/*.h",
  ]),
  # strip_include_prefix = "vendor/irrlicht/", # does not work on Windows
  srcs = glob([
    "source/Irrlicht/*.cpp",
  ]),
  deps = [
    ":libpng",
    ":jpeglib",
    ":lzma",
    ":bzip2",
    ":zlib",
    ":aesGladman",
    ":DirectX",
  ],
  copts = [
    "-DIRRLICHT_EXPORTS=1",
    "-Wall",
    "-pipe",
    "-fno-exceptions",
    "-fno-rtti",
    "-fstrict-aliasing",
    "-O3",
    "-DPNG_THREAD_UNSAFE_OK",
    "-DPNG_NO_MMX_CODE",
    "-DPNG_NO_MNG_FEATURES",
    "-fPIC",
  ],
  visibility = [
    "//visibility:public"
  ],
)

cc_library(
  name = "irrlicht",
  deps = select({
    ":osx": [
      ":irrlicht-osx",
    ],
    ":linux": [
      ":irrlicht-linux",
    ],
    ":windows": [
      ":irrlicht-windows",
    ],
  }),
  includes = [
    "include"
  ],
  visibility = [
    "//visibility:public",
  ],
)
