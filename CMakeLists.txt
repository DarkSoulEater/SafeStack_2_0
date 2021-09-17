cmake_minimum_required(VERSION 3.17)
project(SafeStack_2_0)

set(CMAKE_CXX_STANDARD 17)

add_executable(SafeStack_2_0
        src/main.cpp
        src/global/config.h
        src/global/global.h
        src/tools/stack.cpp
        src/tools/stack.h
        src/global/global.cpp
        src/global/logging.cpp
        src/global/logging.h
        src/tools/memory.cpp
        src/tools/memory.h src/tmp.h src/tools/long_array.cpp src/tools/long_array.h src/global/win.cpp src/global/win.h)
