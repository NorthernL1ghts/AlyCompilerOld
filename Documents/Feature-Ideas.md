# Feature Ideas for Aly

## Core Features

- [ ] Statically typed architecture
- [x] Whitespace agnostic parsing
- [x] Custom comment delimiters (`;` for assembly-like familiarity, `#` for Python-like usage)
- [x] Avoiding semicolon (`;`) as a line terminator (delimiter)
- [x] AT&T x86_64 GNU Assembly code generation

## Future Considerations

- [ ] VSC extension for syntax highlighting and autocompletion
- [ ] Function handling support
- [ ] Pre- and post-operators integration
- [ ] Reserve `;` for potential future language features

## Notes

- Parser setup using `argv[::-1]`
- Linking executable to Windows Subsystem and Console application

---
