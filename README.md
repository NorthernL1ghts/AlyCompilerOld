# AlyCompiler

![AlyCompiler Logo](/Resources/AlyCompilerLogo.png)

I don't really have a plan, I just want to make a compiler; it sounds fun. There is an example of some code that should be valid and able to be compile in [`example`](example) file.

I have decided to call the Language Aly short for `Analytic Lexical Yield`, this is because I've decided to dedicate this language to my partner Alyssa.

## Usage

Run the executable from a shell with a path to some source code as the only argument. Currently, we print out the furthest progress we are able to make. Eventually, we will output compiled source code.

## 🛠 Building Instructions

### Dependencies

- **[CMake](https://cmake.org/)** (Version 3.14 or later)
- **[GCC](https://gcc.gnu.org/)** or any other C Compiler (e.g., Clang, MSVC)

### 1️⃣ Generate Build Tree

Run the following command to generate the build tree:

```sh
cmake -G "Unix Makefiles" -B bld
```

### 2️⃣ Build the Executable

Once the build tree is ready, compile the project using:

```sh
cmake --build bld
```

Alternatively, you can use the provided scripts:

- Windows: `Scripts/Win-GenerateProject.bat`
- Linux/macOS: `Scripts/GenerateProject.sh`
