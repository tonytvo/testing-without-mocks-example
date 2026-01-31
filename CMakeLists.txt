cmake_minimum_required(VERSION 3.20)

project(testing_without_mocks_example
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(cpp)
