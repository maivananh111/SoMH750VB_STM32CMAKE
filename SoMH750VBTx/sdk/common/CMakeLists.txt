cmake_minimum_required(VERSION 3.20)


set(COMMON_NAME common)


project(${COMMON_NAME})
add_library(${COMMON_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${COMMON_NAME} INTERFACE
    include
)
target_sources(${COMMON_NAME} INTERFACE

)
target_link_directories(${COMMON_NAME} INTERFACE)
target_link_libraries(${COMMON_NAME} INTERFACE

)