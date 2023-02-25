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
as target name, so it's always good to have bwLog
stored in separate subfolder
(in this case, /lib/)

### Getting started (no CMake??)

If you don't want to use (or generally just don't use)
CMake, the process of adding depends on your build toolchain, IDE, ...
Generally, you want to add `bwLog/include` into
your project's include paths, and `bwLog/src` into
project's source files. 


### Basic usage

To enable default log handler, use

```c++
bwtk::log::enableDefaultHandler();
```

If you want to use custom handler function, you firstly
need to create a `LogCallback` function.
Example signature of `LogCallback` looks like this:

```c++
void my_callback(bwtk::log::LogSeverity severity, const bwtk::log::TimePoint& tp, const std::string& message);
```

Once you implement your log callback, simply add it 
into bwLog with:

```c++
bwtk::log::addCallback(my_callback);
```

Where `severity` represents log's level, `tp` 
represents TimePoint structure containing information
about when the log was created (currently only time),
and `message` contains the log's message, stored
as standard C++ string.

In order to invoke new Log, you can use:

```c++
using namespace bwtk;
log::Log(log::LogSeverity::Info) << "Hello world!";
```

This will create new info log containing with message
'Hello world!'. If default log callback is enabled, you should see following
message printed in console:

```
[INFO] 20:10:33: Hello world!
```

Note that your time stamp will be probably different from example.

If you don't want to print such a lengthy string all the time,
you can use built-in macro:

```c++
bwLogINFO << "Hello world!";
```

Which will output same message as previous example
(if default log handler is enabled!)