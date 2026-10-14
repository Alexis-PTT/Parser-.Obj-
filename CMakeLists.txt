cmake_minimum_required(VERSION 4.2)
project(parseurTest C)

set(CMAKE_C_STANDARD 17)
include(FetchContent)

FetchContent_Declare(
        alexer
        GIT_REPOSITORY https://github.com/tsoding/alexer.git
        GIT_TAG main
)

FetchContent_GetProperties(alexer)

if(NOT alexer_POPULATED)
    FetchContent_MakeAvailable(alexer)
endif()

add_library(alexer INTERFACE)

target_include_directories(alexer INTERFACE
        ${alexer_SOURCE_DIR}
)

add_executable(parseurTest main.c)

target_link_libraries(parseurTest PRIVATE alexer)
