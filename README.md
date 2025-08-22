# dictools

A minimal header-only C++ library project for dict template, managed by CMake, with GoogleTest-based tests.

## Layout

```
├── cmake
│   └── dictoolConfig.cmake.in
├── CMakeLists.txt
├── include
│   └── dictool
├── Makefile
├── README.md
└── tests
```

## Build

```bash
# Configure
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build library + tests
cmake --build build

# Run tests
ctest --test-dir build --output-on-failure
```

## Install

```bash
cmake --install build --prefix ./install
```

This produces:
```
install/
├─ include/dictool
└─ lib/cmake/mylib/
   ├─ dictoolConfig.cmake
   ├─ dictoolConfigVersion.cmake
   └─ dictoolTargets.cmake
```

## Use as a dependency

After installation, in a consuming CMake project:
```cmake
find_package(dictool REQUIRED)
add_executable(app main.cpp)
target_link_libraries(app PRIVATE dictool::dictool)
```
```