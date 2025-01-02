# applefetch

[![CI](https://github.com/ryouze/applefetch/actions/workflows/ci.yml/badge.svg)](https://github.com/ryouze/applefetch/actions/workflows/ci.yml)
[![Release](https://github.com/ryouze/applefetch/actions/workflows/release.yml/badge.svg)](https://github.com/ryouze/applefetch/actions/workflows/release.yml)
![Release version](https://img.shields.io/github/v/release/ryouze/applefetch)

applefetch is a CLI system information tool for macOS, inspired by [neofetch](https://github.com/dylanaraps/neofetch).


## Motivation

I wanted to create a neofetch clone for macOS that uses native C++ code and, where possible, system calls, rather than relying on the shell.

It looks like this:

```
OS: macOS 14.6.1 (arm64)
Model: MacBookPro18,3
Uptime: 17d 23h 52m
Packages: 138 (brew)
Shell: /bin/zsh
Display: 1512x982 @ 120 Hz
CPU: Apple M1 Pro
Memory: 10.16GiB / 16.00GiB (63%)
```

Please note that more advanced projects (e.g., [fastfetch](https://github.com/fastfetch-cli/fastfetch)) are already available, this is merely a learning exercise for me.


## Features

- Written in modern C++ (C++17).
- Comprehensive documentation with doxygen-style comments.
- Automatic third-party dependency management using CMake's [FetchContent](https://www.foonathan.net/2022/06/cmake-fetchcontent/).
- No missing STL headers thanks to [header-warden](https://github.com/ryouze/header-warden).


## Tested Systems

This project has been tested on the following systems:

- macOS 14.6 (Sonoma)

Automated testing is also performed on the latest version of macOS using GitHub Actions.


## Pre-built Binaries

Pre-built binaries are available for macOS (ARM64). You can download the latest version from the [Releases](../../releases) page.

To remove macOS quarantine, use the following commands:

```sh
xattr -d com.apple.quarantine applefetch-macos-arm64
chmod +x applefetch-macos-arm64
```


## Requirements

To build and run this project, you'll need:

- C++17 or higher
- CMake


## Build

Follow these steps to build the project:

1. **Clone the repository**:

    ```sh
    git clone https://github.com/ryouze/applefetch.git
    ```

2. **Generate the build system**:

    ```sh
    cd applefetch
    mkdir build && cd build
    cmake ..
    ```

    Optionally, you can disable compile warnings by setting `ENABLE_COMPILE_FLAGS` to `OFF`:

    ```sh
    cmake .. -DENABLE_COMPILE_FLAGS=OFF
    ```

3. **Compile the project**:

    To compile the project, use the following command:

    ```sh
    cmake --build . --parallel
    ```

After successful compilation, you can run the program using `./applefetch`. However, it is highly recommended to install the program, so that it can be run from any directory. Refer to the [Install](#install) section below.

**Note:** The mode is set to `Release` by default. To build in `Debug` mode, use `cmake .. -DCMAKE_BUILD_TYPE=Debug`.


## Install

If not already built, follow the steps in the [Build](#build) section and ensure that you are in the `build` directory.

To install the program, use the following command:

```sh
sudo cmake --install .
```

This will install the program to `/usr/local/bin`. You can then run the program from any directory using `applefetch`.


## Usage

To run the program, use the following command:

```sh
applefetch
```

On startup, the program will display system information akin to the following:

```
OS: macOS 14.6.1 (arm64)
Model: MacBookPro18,3
Uptime: 17d 23h 52m
Packages: 138 (brew)
Shell: /bin/zsh
Display: 1512x982 @ 120 Hz
CPU: Apple M1 Pro
Memory: 10.16GiB / 16.00GiB (63%)
```

If the `NO_COLOR` environment variable is set, the program will not use any color codes in the output.

```sh
NO_COLOR=1 applefetch
```


## Flags

```sh
[~] $ applefetch --help
Usage: applefetch [-h] [-v]

CLI system information tool for macOS, inspired by neofetch.

Optional arguments:
  -h, --help     prints help message and exits
  -v, --version  prints version and exits
```


## Testing

Tests are included in the project but are not built by default.

To enable and build the tests manually, run the following commands from the `build` directory:

```sh
cmake .. -DBUILD_TESTS=ON
cmake --build . --parallel
ctest --output-on-failure
```


## Credits

- [fmt](https://github.com/fmtlib/fmt)


## Contributing

All contributions are welcome.


## License

This project is licensed under the MIT License.
