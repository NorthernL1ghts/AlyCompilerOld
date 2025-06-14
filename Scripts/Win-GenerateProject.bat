@echo off

@REM Navigate to the parent directory
cd ..

@REM Configure the project using CMake with the Visual Studio 2022 generator and x64 toolset
cmake -G "Visual Studio 17 2022" -A x64 -B bld -DCMAKE_BUILD_TYPE=Debug

@REM Build the project in Debug mode
cmake --build bld --config Debug