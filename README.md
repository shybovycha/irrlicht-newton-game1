# Newton GD for Irrlicht game developers

## Briefing

This repo contains the code for [Irrlicht + Newton game tutorial](http://shybovycha.github.io/irrlicht-newton-tutorials).

## Changelog

* **9 Nov 2015** Added basic NewtonGD sample.
* **8 Nov 2015** Added Lua scripting and all the interaction logic is now defined by the Lua scripts.
* **7 Nov 2015** Sample Irrlicht program compiles with CMake build system.
* **3 Oct 2017** Migrated to Buck build system.
* **13 Apr 2018** Added many more improvements to the build configuration and some troubleshooting guides.
* **16 Apr 2018** Upgraded to Irrlicht 1.8.4
* **18 Apr 2018** Migrated to Bazel build system

## TODO

* Fix starting position/rotation for bodies when creating them
* Fix impulse/force applying
* ~~Fix OSX window focus~~
* ~~Add automatic `media/` copying to the bundle/binary directory~~
* ~~Generalize build rules for different platforms~~
* ~~Upgrade Irrlicht to `1.8.4`~~
* ~~Upgrade NewtonGD to `3.14`~~

## Step-by-step build

1. install [Bazel](https://bazel.build/) from master branch
2. to build project, run the command from the project root directory

  a. for OSX:

    ```bash
    bazel build //irrlicht-newton-game:irrlicht-newton-game --apple_platform_type=macos
    ```

  b. for Linux & others:

    ```bash
    bazel build //irrlicht-newton-game:irrlicht-newton-game
    ```

3. run a demo with

  a. for OSX:

    ```bash
    bazel run //irrlicht-newton-game:irrlicht-newton-game
    ```

  b. for others:

    ```bash
    bazel run //irrlicht-newton-game:irrlicht-newton-game --apple_platform_type=macos
    ```

## Important hints & troubleshooting

### Newton bits

There is no more `GetIdentityMatrix()` - it was replaced with `dGetIdentityMatrix()`.
There's also no more need to set `m_w` for `dVector` instances - it has the default value of `1.0`.
You can not multiply a `dVector` by a number - you have to call `vector.Scale(x)` to do this.
There are no `NewtonSetPlatformArchitecture` and `NewtonSetWorldSize` anymore.

### Clang 6.0 compilation errors for Irrlicht

I've ran into two error messages after an upgrade of Clang to 6.0:

```
vendor/irrlicht/source/Irrlicht/MacOSX/CIrrDeviceMacOSX.mm:645:7: error: cast from pointer to smaller type 'NSOpenGLPixelFormatAttribute' (aka 'unsigned int') loses information
                                                (NSOpenGLPixelFormatAttribute)nil
                                                ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
vendor/irrlicht/source/Irrlicht/MacOSX/CIrrDeviceMacOSX.mm:671:26: error: cast from pointer to smaller type 'NSOpenGLPixelFormatAttribute' (aka 'unsigned int') loses information
                                                        windowattribs[14]=(NSOpenGLPixelFormatAttribute)nil;
```

The fix was relatively easy, but required modifying the Irrlicht sources, which makes me sad.

```objc
// before:
windowattribs[14]=(NSOpenGLPixelFormatAttribute)nil;

// after:
windowattribs[14]=0;
```


```objc
// before:
NSOpenGLPixelFormatAttribute windowattribs[] =
{
    NSOpenGLPFANoRecovery,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)depthSize,
    NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute)CreationParams.Bits,
    NSOpenGLPFAAlphaSize, (NSOpenGLPixelFormatAttribute)alphaSize,
    NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
    NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)CreationParams.AntiAlias,
    NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute)(CreationParams.Stencilbuffer?1:0),
    NSOpenGLPFADoubleBuffer,
    (NSOpenGLPixelFormatAttribute)nil
};

// after:
NSOpenGLPixelFormatAttribute windowattribs[] =
{
    NSOpenGLPFANoRecovery,
    NSOpenGLPFAAccelerated,
    NSOpenGLPFADepthSize, (NSOpenGLPixelFormatAttribute)depthSize,
    NSOpenGLPFAColorSize, (NSOpenGLPixelFormatAttribute)CreationParams.Bits,
    NSOpenGLPFAAlphaSize, (NSOpenGLPixelFormatAttribute)alphaSize,
    NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
    NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)CreationParams.AntiAlias,
    NSOpenGLPFAStencilSize, (NSOpenGLPixelFormatAttribute)(CreationParams.Stencilbuffer?1:0),
    NSOpenGLPFADoubleBuffer,
    0
};
```

### Window is not shown on OSX

Change one line in ObjectiveC code of Irrlicht, in the `CIrrDeviceMacOSX.mm` file,
the `CIrrDeviceMacOSX::CIrrDeviceMacOSX` method (constructor):

```objc
// before:
[NSApplication sharedApplication]

// after:
[[NSApplication sharedApplication] activateIgnoringOtherApps:YES];
```

And then rebuild the application.

### How can I pack the OSX application?

First, you will need to alter all the paths in the application code (both in C++ code and the scripts) to
use the paths relative to the parent directory, containing your application bundle (the `.app/` directory):

```cpp
scriptMgr->loadScript("irrlicht-newton-game.app/Contents/MacOS/media/media/media/scripts/test1.lua");
```

and

```cpp
createMesh("colliseum", "irrlicht-newton-game.app/Contents/MacOS/media/media/media/models/ramp_floor.3ds", "./media/media/media/models/ramp_floor.png")
```

Then run these commands to create the bundle itself:

```bash
cd bazel-bin/irrlicht-newton-game/irrlicht-newton-game.runfiles/__main__/irrlicht-newton-game
mkdir -p irrlicht-newton-game.app/Contents/MacOS
cp -r media/ irrlicht-newton-game irrlicht-newton-game.app/Contents/MacOS
```

Now you might run your application using either Finder or `open irrlicht-newton-game.app`.