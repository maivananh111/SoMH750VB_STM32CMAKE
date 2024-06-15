cmake_minimum_required(VERSION 3.20)


set(PROJECT_NAME components_cmsis)

project(${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${PROJECT_NAME} INTERFACE
	.
)
target_sources(${PROJECT_NAME} INTERFACE

)
target_link_directories(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE components_hal)