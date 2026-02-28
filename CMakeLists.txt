cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyProject
        src/main.cpp
        src/Decoder/decoder.cpp
)

target_include_directories(MyProject PRIVATE src)