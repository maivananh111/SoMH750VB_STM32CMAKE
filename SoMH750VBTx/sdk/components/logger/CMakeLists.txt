cmake_minimum_required(VERSION 3.20)


set(COMPONENTS_NAME logger)


project(${COMPONENTS_NAME})
add_library(${COMPONENTS_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${COMPONENTS_NAME} INTERFACE
    .
)
target_sources(${COMPONENTS_NAME} INTERFACE
	logger.c
)
target_link_directories(${COMPONENTS_NAME} INTERFACE)
target_link_libraries(${COMPONENTS_NAME} INTERFACE
	freertos
	driver_hal
)