cmake_minimum_required(VERSION 3.20)


set(LOGGER_NAME logger)


project(${LOGGER_NAME})
add_library(${LOGGER_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${LOGGER_NAME} INTERFACE
    .
)
target_sources(${LOGGER_NAME} INTERFACE
	logger.c
)
target_link_directories(${LOGGER_NAME} INTERFACE)
target_link_libraries(${LOGGER_NAME} INTERFACE
	freertos
	driver_hal
)