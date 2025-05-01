# AlyCompiler

![AlyCompiler Logo](/Resources/AlyCompilerLogo.png)

I don't really have a plan, I just want to make a compiler; it sounds fun. There's is an example of some code that should be valid and able to be compile in [`example`](examply.aly) file.

## Usage

Running the compiler executable with no arguments will display a usage message that contains compiler flags and options as well as command layout.

For writing in Aly in Emacs or Vim, there are helpful plugins in the Editor subdirectory.
For syntax highlighting in any Editor that supports tree-sitter, there is a tree-sitter parser that can be found [here](https://github.com/NorthernL1ghts/tree-sitter-int).
It is highly recommended to use tree-sitter if you can, as it provides much better syntax highlighting.

## 🛠 Building Instructions

### Dependencies

- **[CMake](https://cmake.org/)** (Version 3.14 or later)
- **[GCC](https://gcc.gnu.org/)** or any other C Compiler (e.g., Clang, MSVC)

NOTE: If on Windows and using Visual Studio, see this document [here](https://github.com/NorthernL1ghts/AlyCompiler/blob/main/Docs/VISUAL_STUDIO.org) instead.

### 1️⃣ Generate Build Tree

First, generate a build tree using CMake.

```sh
cmake -B bld
```

### 2️⃣ Build the Executable

Finally, build an executable from the build tree.

```sh
cmake --build bld
```

Alternatively, you can use the provided scripts:

- Windows: `Scripts/Win-GenerateProject.bat`
- Linux/macOS: `Scripts/GenerateProject.sh`

3️⃣ Building Generated x86_64 ASM on Windows under MinGW
```sh
as code.S -o code.o
ld code.o -o code.exe && code.exe
```

TODO: We have to work on getting Linux to work with this project, nothing is preventing it -- Just means we need to use VM.
