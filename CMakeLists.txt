# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.14)

# Use 'ccache' if it is installed in the system's PATH.
# ccache is a compiler cache that speeds up recompilation by caching previous compilations
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Export compilation database into JSON format.
# This generates a compile_commands.json file which is useful for tools like clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Define the project name and programming language
project(AlyCompiler C)

# Create an executable target named 'alyc' from the following source files
add_executable(
    alyc
    src/file_io.c    # File input/output operations
    src/codegen.c    # Code generation functionality
    src/error.c      # Error handling
    src/main.c       # Main program entry point
    src/parser.c     # Parser implementation
    src/environment.c # Environment/context management
)

# Add the src directory to the include path for the alyc target
# This allows the compiler to find header files in the src directory
target_include_directories(
    alyc
    PUBLIC src/
)