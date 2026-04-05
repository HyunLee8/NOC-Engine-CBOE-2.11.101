cmake_minimum_required(VERSION 3.16)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(MyProject
        src/main.cpp
        src/Decoder/decoder.cpp
        src/Messages/NewOrderCrossMsg.cpp
        src/Serialization/Serialization.cpp
        src/Utils/HexConversion.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(MyProject
        nlohmann_json::nlohmann_json
)

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