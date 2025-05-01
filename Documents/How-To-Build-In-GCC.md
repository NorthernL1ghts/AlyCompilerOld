## Building and Running AlyCompiler on Windows (MinGW + GCC)

This document details the steps required to build and execute the AlyCompiler project on a Windows system using the MinGW (Minimalist GNU for Windows) environment, which includes the GCC compiler, along with the CMake build system.

### 1. Prerequisites

* **Install MSYS2:**
  * **Download:** Visit the official MSYS2 website: [https://www.msys2.org/](https://www.msys2.org/)
  * **Installation:** Run the downloaded installer and follow the on-screen instructions. It's recommended to install it in a directory without spaces in the path (e.g., `C:\msys64`).

* **Install MinGW and CMake:**
  * **Open MSYS2 MinGW 64-bit Shell:** Launch `mingw64.exe` from your MSYS2 installation directory.
  * **Update Packages:**
      ```bash
      pacman -Syu
      ```

      - You might need to run this command twice, closing the shell in between if prompted.
  * **Install GCC (C Compiler):**

      ```bash
      pacman -S mingw-w64-x86_64-gcc
      ```

  * **Install CMake (Cross-Platform Build System):**

      ```bash
      pacman -S mingw-w64-x86_64-cmake
      ```

  * **Package Manager (`pacman`):** For more information, see the MSYS2 `pacman` wiki: [https://www.msys2.org/wiki/Pacman/](https://www.msys2.org/wiki/Pacman/)

### 2. Building AlyCompiler

* **Navigate to the Project Source Directory:**

    ```bash
    cd /d/AlyCompiler
    ```

  * **Important:** Replace `/d/AlyCompiler` with the actual path to your AlyCompiler project root. MSYS2 uses Unix-style paths for Windows drives (e.g., `C:` becomes `/c/`, `D:` becomes `/d/`).

* **Generate Build Files and Initiate the Build Process:**

    ```bash
    cmake -S . -B bld -G "MinGW Makefiles" && cmake --build bld
    ```

  * `cmake`: The command-line tool for CMake.
  * `-S .`: Specifies the current directory as the source directory.
  * `-B bld`: Creates (or uses) the `bld` directory for build output.
  * `-G "MinGW Makefiles"`: Tells CMake to generate build files for MinGW's `make` utility.
  * `&&`: Shell operator to execute the next command only if the previous one succeeds.
  * `cmake --build bld`: Starts the compilation process using the generated build files in the `bld` directory.

### 3. Running AlyCompiler

* After a successful build, the `alyc.exe` executable will be in the `bld` subdirectory.
  * **Navigate to the `bld` Directory:**

      ```bash
      cd bld
      ```

  * **Execute the Executable:**

      ```bash
      ./alyc.exe <path_to_file_to_compile>
      ```

      - The `./` prefix is needed in Unix-like shells to run an executable in the current directory.

By following these steps, you should be able to build and run the AlyCompiler project on your Windows system using MinGW and CMake. Remember to adjust the directory paths as needed for your specific setup.
