cmake_minimum_required(VERSION 3.16.3)

project(CT-compiler VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 14)

set(C_FLAGS 
    -Wall
    -o3
)

add_library(${PROJECT_NAME}-main SHARED
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}-main PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    /include
)

target_compile_options(${PROJECT_NAME}-main
    PRIVATE
        ${C_FLAGS}
)


add_executable(${PROJECT_NAME}
    $<TARGET_OBJECTS:${PROJECT_NAME}-main>
    ${HEADERS_LIST}
    ${SOURCES_LIST}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    /include
)

target_compile_options(${PROJECT_NAME}
    PRIVATE
        ${C_FLAGS}
)

