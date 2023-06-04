## Installation

1. Clone the repository via `git clone https://github.com/geugenm/basic-game-engine.git`
NOTE: CMake will automatically download all required submodules and dependencies, [implemented here.](https://github.com/geugenm/basic-game-engine/blob/main/cmake/git_update_submodules.cmake)

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
you can use an automized [build.sh](https://github.com/geugenm/sdl-overview/blob/master/scripts/build.sh)
in `scripts` folder. This is universal script, so when launched type `..` to cd upper, to the project root by default,
and then you'll need to type the build type from the list that will be printed (case is important, type as it printed).

### Docker build on manjaro

```bash
docker build -t my-image -f docker/Dockerfile .
```

## Documentation

All you need is to install doxygen and run these commands:

```bash
doxygen docs/.doxygen
cd scripts
sh build_docs.sh
```

It will build docs in `build/docs`. After that you can build docs with latex if required manually via `make` command in `build/docs/latex/`.

## Contributing

If you'd like to contribute to this project, feel free to fork the repository and submit a pull request with your
changes.Contributions are always welcome, whether it's an improvement to the existing program or a new program in a
different language.

## License

This project is licensed under the AGPL-3.0 License — see the [`LICENSE`](https://github.com/geugenm/basic-game-engine/blob/main/LICENSE.md) file for details.

## Acknowledgments

Thanks to the developers of CMake and CTest for providing the tools necessary to build and test this program. Also,
thanks to the open-source community for providing resources and support for C++ development.