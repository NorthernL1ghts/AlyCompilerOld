# ASM-Compilation-Guide - How to Generate Executable Code from AT&T x86_64 GNU Assembly
=====================================================================================

This document provides detailed instructions for converting assembly source files
(written in AT&T syntax for the x86_64 architecture) into executable binaries.
This process is useful when working with the AlyCompiler project to integrate or test low-level code generation.

-------------------------------------------------------------------------------

## Prerequisites
-------------
You must have the GNU toolchain installed, including:
- GNU Assembler (`as`) for converting assembly code into object files
- GNU Linker (`ld`) for linking the object file into an executable

These tools are available through distributions like MinGW (on Windows) or GCC (on Linux/MSYS2).

-------------------------------------------------------------------------------

## Step-by-Step Instructions
-------------------------

### 1. Assemble the Source File
   -------------------------
   Convert your `.S` assembly source file into an object file:

    ```bash
    as code.S -o code.o
    ```

    - `as`: GNU assembler
    - `code.S`: Your assembly source
    - `-o code.o`: Output object file

### 2. Link the Object File
   ---------------------
   Link the object file into an executable (console app for Windows):

    ```bash
    ld code.o -subsystem=console
    ```

    This creates an `a.exe` by default.

### 3. Specify the Output Executable Name (Optional)
   ---------------------------------------------
   To name the output executable:

    ```bash
    ld code.o -subsystem=console -o code.exe
    ```

-------------------------------------------------------------------------------

## Platform-Specific Notes
-----------------------

- **On Windows:**
  - Use `-subsystem=console` for terminal-based apps.
  - Use `-subsystem=windows` if building a GUI-based application (e.g., with `WinMain`).

- **On Linux:**
  - No `-subsystem` is needed.
  - Example:

    ```bash
    as code.S -o code.o
    ld code.o -o code
    ```

-------------------------------------------------------------------------------

## Summary
-------

| Step          | Command                                       | Description                      |
|---------------|-----------------------------------------------|----------------------------------|
| Assemble      | `as code.S -o code.o`                           | Converts assembly to object code|
| Link          | `ld code.o -subsystem=console`                  | Links into executable            |
| Custom Output | `ld code.o -subsystem=console -o code.exe`      | Names the executable explicitly  |

-------------------------------------------------------------------------------

By following this guide, you can generate executable binaries from AT&T-style
GNU assembly code and integrate them with AlyCompiler development flows.