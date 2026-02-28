cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyProject
        src/main.cpp
        src/Decoder/decoder.cpp
        src/Messages/NewOrderCrossMsg.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(MyProjectTests
        tests/decoder_test.cpp
        src/Decoder/decoder.cpp
        src/Messages/NewOrderCrossMsg.cpp
)

target_include_directories(MyProjectTests PRIVATE src)

target_link_libraries(MyProjectTests
        gtest
        gtest_main
)