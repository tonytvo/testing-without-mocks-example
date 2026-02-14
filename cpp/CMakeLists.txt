cmake_minimum_required(VERSION 3.20)

project(testing_without_mocks_example_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(CTest)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)

add_library(project_warnings INTERFACE)
if(MSVC)
    target_compile_options(project_warnings INTERFACE
        /W4
        /permissive-
        /Zc:preprocessor
        /Zc:__cplusplus
        /EHsc
    )
else()
    target_compile_options(project_warnings INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wnon-virtual-dtor
    )
endif()

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE BOOL "Don't update dependencies automatically" FORCE)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)

FetchContent_MakeAvailable(Catch2)

add_library(rot13_logic
    src/logic/Rot13.cpp
)
target_include_directories(rot13_logic
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(rot13_logic
    PUBLIC project_options
    PRIVATE project_warnings
)

if(BUILD_TESTING)
    enable_testing()

    add_executable(rot13_logic_tests
        tests/logic/Rot13Tests.cpp
    )
    target_link_libraries(rot13_logic_tests
        PRIVATE
            rot13_logic
            Catch2::Catch2WithMain
            project_options
            project_warnings
    )
    add_test(NAME rot13_logic_tests COMMAND rot13_logic_tests)
endif()
