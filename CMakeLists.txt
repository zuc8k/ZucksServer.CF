cmake_minimum_required(VERSION 3.20)
project(ZucksServer VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ZUCKS_BUILD_TESTS "Build Zucks tests" OFF)

include(FetchContent)

set(ASIO_STANDALONE ON CACHE BOOL "" FORCE)
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-28-0
)
FetchContent_MakeAvailable(asio)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

find_package(Threads REQUIRED)

add_executable(zucks-server
    src/main.cpp
    src/zucks_core.cpp
    src/zucks_proxy.cpp
    src/zucks_filter.cpp
    src/zucks_logger.cpp
    src/zucks_config.cpp
)

target_include_directories(zucks-server PRIVATE
    src
    include
    ${asio_SOURCE_DIR}/include
)

target_compile_definitions(zucks-server PRIVATE ASIO_STANDALONE)

target_link_libraries(zucks-server PRIVATE
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    Threads::Threads
)

if(ZUCKS_BUILD_TESTS)
    enable_testing()

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(zucks-tests
        tests/test_zucks_filter.cpp
        src/zucks_filter.cpp
    )
    target_include_directories(zucks-tests PRIVATE src)
    target_link_libraries(zucks-tests PRIVATE GTest::gtest_main Threads::Threads)

    include(GoogleTest)
    gtest_discover_tests(zucks-tests)
endif()