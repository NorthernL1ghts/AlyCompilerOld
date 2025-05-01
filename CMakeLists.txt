# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Use 'ccache' if available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Export compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Define the project
project(AlyCompiler C)

# Create an executable target
add_executable(
    alyc
    src/file_io.c
    src/codegen.c
    src/error.c
    src/main.c
    src/parser.c
    src/environment.c
)

# Include path setup
target_include_directories(
    alyc
    PUBLIC src/
)

# Compiler options for GCC and Clang (strict warnings)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(
        # alyc
        # PRIVATE
        # -Wall
        # -Wextra
        # -pedantic
        # -Werror  # Treat warnings as errors
    )
endif()

# Suppress specific MSVC warnings while keeping /W4 for general checks
if(MSVC)
    target_compile_options(
        # alyc
        # PRIVATE
        # /W4        # High warning level
        # /WX        # Treat warnings as errors
        # /wd4996    # Suppress 'unsafe' functions like sprintf
        # /wd4189    # Suppress 'unused variable' warnings
        # /wd4456    # Suppress shadowed variable warnings
        # /wd4477    # Suppress printf format specifier mismatches
    )

    # Define MSVC-specific macros
    target_compile_definitions(alyc PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()