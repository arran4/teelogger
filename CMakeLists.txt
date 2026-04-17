cmake_minimum_required(VERSION 3.10)

project(teelogger VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Define sources
set(SOURCES
    src/main.c
    src/logger.c
)

# Create executable
add_executable(teelogger ${SOURCES})

# Add include directory to target
target_include_directories(teelogger PRIVATE include)

# Installation rules
install(TARGETS teelogger DESTINATION bin)
install(FILES man/teelogger.1 DESTINATION share/man/man1)
