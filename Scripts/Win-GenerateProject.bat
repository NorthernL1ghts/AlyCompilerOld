@echo off
cd ..
cmake -G "Visual Studio 17 2022" -A x64 -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
