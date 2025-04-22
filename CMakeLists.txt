cmake_minimum_required(VERSION 3.14)

# Use 'ccache' if it is installed in the system's PATH.
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Export compilation database into JSON format.
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(AlyCompiler C)

add_executable(
    alyc
    src/file_io.c
    src/codegen.c
    src/error.c
    src/main.c
    src/parser.c
    src/environment.c
)

target_include_directories(
    alyc
    PUBLIC src/
)