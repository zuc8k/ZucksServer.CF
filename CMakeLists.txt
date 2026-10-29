cmake_minimum_required(VERSION 3.20)
project(ZucksServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# خيارات المنصة
option(ZUCKS_BUILD_TESTS "Build Zucks unit tests" OFF)
option(ZUCKS_ENABLE_DOCKER "Enable Docker support" ON)

# المكتبات الخارجية (Asio, spdlog, nlohmann_json)
include(FetchContent)
FetchContent_Declare(asio GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git GIT_TAG asio-1-28-0)
FetchContent_Declare(spdlog GIT_REPOSITORY https://github.com/gabime/spdlog.git GIT_TAG v1.13.0)
FetchContent_Declare(json GIT_REPOSITORY https://github.com/nlohmann/json.git GIT_TAG v3.11.3)
FetchContent_MakeAvailable(asio spdlog json)

# الملفات التنفيذية
add_executable(zucks-server
    src/main.cpp
    src/zucks_core.cpp
    src/zucks_proxy.cpp
    src/zucks_filter.cpp
    src/zucks_logger.cpp
    src/zucks_config.cpp
)
target_include_directories(zucks-server PRIVATE include)
target_link_libraries(zucks-server PRIVATE asio::asio spdlog::spdlog nlohmann_json::nlohmann_json)

if(ZUCKS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()