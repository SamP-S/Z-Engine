# Zen Agent Guidelines

## Build Commands

### Full Build
```bash
./compile.sh          # Standard build (Debug)
./compile.sh clean    # Clean then rebuild
```

### Manual CMake Commands
```bash
cmake -S . -B build   # Configure
cmake --build build   # Build
cmake --build build --target install  # Install
```

### Build Options
Toggle these in CMakeLists.txt or pass to cmake:
- `BUILD_CORE`: Build core library (ON by default)
- `BUILD_MODULES`: Build all modules (ON by default)
- `BUILD_ENGINE`: Build engine targets (OFF by default)
- `BUILD_PROJECTS`: Build sample projects (OFF by default)
- `BUILD_EXAMPLES`: Build example apps (OFF by default)

## Code Style Guidelines

### Formatting
- Follows Google C++ style with modifications (see `.clang-format` in spdlog)
- Use tabs, 4 spaces
- Column limit: 100 characters
- Standard: C++20
- Sort includes: Never (preserve logical grouping)

### Naming Conventions
- Classes/Structs: PascalCase (e.g., `UUID`)
- Variables/Functions: snake_case (e.g., `_trueRandom`)
- Namespaces: snake_case (e.g., `zen::core`)
- Constants: kConstantName or ALL_CAPS for macros
- Template parameters: PascalCase (e.g., `typename T`)

### Import/Include Guidelines
- Standard library includes first (angle brackets)
- Third-party includes second (angle brackets)
- Project includes last (quotes)
- Group related includes together
- Use `#pragma once`

### Type Usage
- Use fixed-width integers from `<cstdint>` for clarity (e.g., `int64_t`, `uint32_t`)
- Prefer `std::size_t` for sizes and counts
- Use `bool` for boolean values
- Use `auto` for complex types when it improves readability
- Prefer references over pointers when null is not valid
- Use `const` liberally for parameters and variables that don't change

### Error Handling
- Use exceptions sparingly and only for truly exceptional conditions
- Prefer error codes for expected failure cases
- Validate inputs at function boundaries, use if guards
- Use assertions (`assert()`) for precondition/postcondition checks in debug builds
- Log warnings and above priority using spdlog liberally

### Memory Management
- Prefer raw pointers
- Avoid manual `new`/`delete` except in low-level systems
- Follow RAII principles strictly

### Comments
- Explain why, not what
- Keep comments up-to-date with code changes
- Use Doxygen-style comments for public APIs
- Remove commented-out code

### Specific to Zen
- All core functionality resides in `zen` namespace, with sub-namespaces for layers
- Core types (like UUID) are in `zen::core`
- Modules follow similar namespacing pattern (e.g., `zen::graphics`)
- Engine systems use appropriate sub-namespaces
- Header files in `include/` directory, source in `src/`

