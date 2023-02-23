# ByteWolf's logging library

### A super-simple, lightweight, modern C++ logging library

ByteWolf's logging library (bwLog for short)
is a simple logging library for C++.

### Features:

- log filtering

- log callbacks for custom log processing

- multi-threading ready

### Requirements

- C++ 17 or higher

- CMake (optional, but highly recommended,
as it makes importing so much easier)


### Getting started (CMake method)

You can start by adding bwLog as submodule into your git
based project:

```
super_cool_project/libs$ git submodule add https://github.com/bytewolfexe/bwLog.git
```

Once added successfully, open your project's
CMakeLists.txt and add following lines
(assuming your project is named super_cool_project, you cloned bwLog into super_cool_project/lib/ 
and CMakeLists.txt is located in super_cool_project/):

```cmake
add_subdirectory(libs/bwLog)

target_include_directories(super_cool_project PUBLIC bwLog)
target_link_libraries(super_cool_project bwLog)
```

Note that CMake may confuse bwLog as folder name and bwLog
as target name, so it's always good to have it in separate subfolder
(in this case, /lib/)

### Getting started (no CMake??)

If you don't want to use (or generally just don't use)
CMake, the process of adding depends on your build toolchain, IDE, ...
