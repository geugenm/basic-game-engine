<h1 align="center">basic-game-engine</h1>
<hr>

<div align="center" style="text-align: center;">
  <div>
    <a href="https://github.com/geugenm/advanced-hello-world/issues">
      <img src="https://img.shields.io/github/issues-raw/geugenm/advanced-hello-world?style=for-the-badge" alt="GitHub Issues">
    </a>
    <img src="https://img.shields.io/badge/platform-linux%20-informational?style=for-the-badge&amp;logo=appveyor" alt="linux">
    <img src="https://img.shields.io/badge/platform-android%20-informational?style=for-the-badge&amp;logo=android" alt="android">
    <img src="https://img.shields.io/badge/platform-windows%20-informational?style=for-the-badge&amp;logo=windows" alt="windows">
    <a href="https://en.cppreference.com/w/cpp/17">
      <img src="https://img.shields.io/badge/cpp-17-informational?style=for-the-badge&amp;logo=cplusplus" alt="CPP version">
    </a>
  </div>
</div>

## What does this engine have?

### OpenGL, SDL

The engine uses OpenGL for rendering and SDL for window management, input, and
audio. OpenGL (Open Graphics Library) is a cross-language, cross-platform API
for rendering 2D and 3D vector graphics. SDL (Simple DirectMedia Layer) is a
cross-platform development library designed to provide low level access to
audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D.

### EnTT, ECS

EnTT is used to implement ECS (Entity Component System) architecture. EnTT is a
header-only, tiny and easy to use library for game programming and much more
written in modern C++, mainly known for its innovative entity-component-system (
ECS) model (skypjack.github.io). ECS is a design pattern often used in game
development where every entity consists of one or more components which add
behaviour or functionality, and systems that are responsible for updating the
entities (skypjack.github.io).

### Sprite Editor

The engine currently only has a sprite editor. A sprite editor allows the user
to manipulate and construct the visual representation of objects within the
game. It allows for the creation, import, and manipulation of 2D images (
sprites) which are commonly used in 2D games.

#### Here's an example of the animation played by the engine:

![](https://i.gifer.com/2huh.gif)

### Simple Music Player 3.0.0 (beta)

The engine has a simple music player with only one file load capability. This
means it can load and play one audio file at a time. The audio file can be used
for background music, sound effects, or any other audio requirements of the
game.

### C++

The entire engine is written in C++, a language that provides high performance
and fine control over system resources. This makes it a popular choice for game
development, where performance is often critical.

The engine is basic and currently only provides essential features. However, it
offers a solid foundation on which more functionalities can be built. Future
improvements could include adding support for multiple audio file loading and
playback, extending the sprite editor, and implementing more advanced ECS
features with EnTT.

<hr>

## Installation

1. Clone the repository
   via `git clone https://github.com/geugenm/basic-game-sdl_gl_engine.git`
   NOTE: CMake will automatically download all required submodules and
   dependencies, [implemented here.](https://github.com/geugenm/basic-game-sdl_gl_engine/blob/main/cmake/git_update_submodules.cmake)

## Build & Deployment

### Build the project

```bash
cmake --preset=Release .
cd build/release
cmake --build . --config Release
```

For more build configurations
see [CMakePresets.json](https://github.com/geugenm/sdl-overview/blob/master/CMakePresets.json)

Also,
you can use an
automized [build.sh](https://github.com/geugenm/sdl-overview/blob/master/scripts/build.sh)
in `scripts` folder. This is universal script, so when launched type `..` to cd
upper, to the project root by default,
and then you'll need to type the build type from the list that will be printed
(the case is important, type as it printed).

## Windows build

The fastest way to compile this project is
to download [clang c++ compiler](https://releases.llvm.org/download.html) and
using CLion IDE from Jetbrains compile it making sure clang compiler is used.
Also, you can use manual generating as
mentioned [earlier](https://github.com/geugenm/basic-game-engine/blob/main/README.md#L74).

## Android notes

If you want
to use a newer version of SDL, make sure you will copy its android project to a
current android project
and adding all the necessary libraries
to [getLibraries()](https://github.com/geugenm/basic-game-engine/blob/main/android-project/app/src/main/java/mil/eugene/gamedev/GameDevSimulator.java#L7)
function.

### Docker build on manjaro

```bash
docker build -t my-image -f docker/Dockerfile .
```

## Documentation

All you need is to install doxygen & LaTeX and run those commands:

```bash
doxygen docs/.doxygen
cd scripts
sh build_docs.sh
```

It'll build docs in `build/docs`. After that you can build docs with latex if
required manually via `make` command in `build/docs/latex/`.

## Contributing

If you'd like to contribute to this project, feel free to fork the repository
and submit a pull request with your
changes.
Contributions are always welcome,
whether it's an improvement to the existing program or a new program in a
different language.

## License

This project is licensed under the AGPL-3.0 License — see
the [`LICENSE`](https://github.com/geugenm/basic-game-sdl_gl_engine/blob/main/LICENSE.md)
file for details.

## Acknowledgments

Thanks to the developers of CMake and CTest for providing the tools necessary to
build and test this program. Also,
thanks to the open-source community for providing resources and support for C++
development.