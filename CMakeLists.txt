cmake_minimum_required(VERSION 3.0...3.27)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(Haffman
        VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    )