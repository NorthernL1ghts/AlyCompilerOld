# LinkNotes - Assembling and Linking Assembly Code

Once you have written your assembly code, you need to assemble and link it to create an executable.

First, assemble the source file into an object file:

```bash
as code.S -o code.o
```

This command translates the assembly source `(code.S)` into an object file `(code.o)`.

Once the object file is ready, proceed with linking:
```bash
ld code.o -L"C:/MinGW/lib" -lmsvcrt -o code.exe
```
