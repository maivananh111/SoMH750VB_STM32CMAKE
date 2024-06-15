cmake_minimum_required(VERSION 3.20)


set(PROJECT_NAME components_hal)
file(GLOB HAL_SOURCE_FILES "*.c")


project(${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${PROJECT_NAME} INTERFACE
    include
    include/Legacy
)
target_sources(${PROJECT_NAME} INTERFACE
	${HAL_SOURCE_FILES}
)
target_link_directories(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE)