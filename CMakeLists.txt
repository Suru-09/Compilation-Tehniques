cmake_minimum_required(VERSION 3.16.3)

project(CT-compiler VERSION 1.0)

include(sources.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

set(C_FLAGS 
    -Wall
    -o3
    -ggdb
)

add_executable(${PROJECT_NAME}
    ${HEADERS_LIST}
    ${SOURCES_LIST}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/symbolTable
    ${CMAKE_SOURCE_DIR}/include/virtualMachine
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/symbolTable
    ${CMAKE_SOURCE_DIR}/src/virtualMachine
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        ${C_FLAGS}
)
