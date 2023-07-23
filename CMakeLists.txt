cmake_minimum_required(VERSION 3.8)

project(koboldcpp_tokenizers)

set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(-std=c++11)

if (DEFINED EMSCRIPTEN)
    add_compile_options(-fno-exceptions) # TODO: also -fno-rtti if we can get rid of -lembind
    add_compile_options(-O3)
    add_link_options(-O3)

    # TODO: see if we can eliminate the need for -lembind
    add_link_options(-lembind -sENVIRONMENT=web --no-entry)

    # TODO: closure is aggressive when pruning things, there's a way to tell it to ignore certain functions
    # add_link_options(-sMODULARIZE --closure 1)

    add_executable(kcpp_tokenizers src/extracted.cpp src/lib.cpp)

    # TODO: the .js is a pile of helpers and converters, but I bet we could just
    # compile a .wasm and use it directly
    set(CMAKE_EXECUTABLE_SUFFIX ".js")
else()
    add_library(kcpp_tokenizers SHARED src/extracted.cpp src/lib.cpp)
endif()
