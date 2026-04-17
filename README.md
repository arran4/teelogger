# teelogger

**Log rotation and tee built into one app.**

`teelogger` is a utility designed to intercept standard input, split the output to standard output, and write it to log files with automatic log rotation built-in.

## Prerequisites

To build this project, you need:
- A C compiler that supports the C11 standard (e.g., GCC, Clang)
- CMake 3.10 or higher

## Build Instructions

`teelogger` uses CMake as its build system. To configure and build the project, run:

```bash
# Configure the build directory
cmake -B build

# Build the executable
cmake --build build
```

This will generate the `teelogger` executable inside the `build/` directory.

## Usage

Once built, you can run the executable from the build directory.

```bash
./build/teelogger [OPTIONS]...
```

### Options

*   `-h` : Print the help message and exit.
*   `-b` : Run in background mode (daemonize).
*   `-f <file>` : Specify the output file to log to.

### Examples

**Print the help message:**
```bash
./build/teelogger -h
```

**Run and log to a specific file:**
```bash
./build/teelogger -f /var/log/my_app.log
```

## Contributing
To ensure consistent coding style, please use `clang-format` before committing any code changes:
```bash
clang-format -i src/*.c include/*.h
```
