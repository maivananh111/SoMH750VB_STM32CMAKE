cmake_minimum_required(VERSION 3.20)


set(PROJECT_NAME components_system)


project(${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${PROJECT_NAME} INTERFACE
    include
)
target_sources(${PROJECT_NAME} INTERFACE
	system_interrupts.c
	freertos_port.c
)
target_link_directories(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE
	components_freertos
	components_hal
)