# Newton GD for Irrlicht game developers

## Briefing

This repo contains the code for [Irrlicht + Newton game tutorial](http://shybovycha.github.io/irrlicht-newton-tutorials).

## Changelog

* **9 Nov 2015** Added basic NewtonGD sample.
* **8 Nov 2015** Added Lua scripting and all the interaction logic is now defined by the Lua scripts.
* **7 Nov 2015** Sample Irrlicht program compiles with CMake build system.
* **3 Oct 2017** Migrated to Buck build system.

## TODO

* Fix starting position/rotation for bodies when creating them
* Fix impulse/force applying
* ~~Fix OSX window focus~~
* Add automatic `media/` copying to the bundle/binary directory
* Generalize build rules for different platforms

## Step-by-step build

1. install [Buck](https://buckbuild.com/) from master branch
2. to build project, run the command from the project root directory
    a. for OSX:

        buck build //irrlicht-newton-game:bundle-osx\#macosx-x86_64

    b. for Linux & others:

        buck build //irrlicht-newton-game:binary-generic

3. copy the `media/` directory to the build directory
4. run a demo with

    a. for OSX:

        buck run //irrlicht-newton-game:bundle-osx\#macosx-x86_64

    b. for others:

        buck run //irrlicht-newton-game:binary-generic

## Important hints

### Newton bits

There is no more `GetIdentityMatrix()` - it was replaced with `dGetIdentityMatrix()`.
There's also no more need to set `m_w` for `dVector` instances - it has the default value of `1.0`.
You can not multiply a `dVector` by a number - you have to call `vector.Scale(x)` to do this.
There are no `NewtonSetPlatformArchitecture` and `NewtonSetWorldSize` anymore.
