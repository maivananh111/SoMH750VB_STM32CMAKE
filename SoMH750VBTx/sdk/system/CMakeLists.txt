cmake_minimum_required(VERSION 3.20)


set(SYSTEM_NAME system)

project(${SYSTEM_NAME})
add_library(${SYSTEM_NAME} INTERFACE)
enable_language(C CXX ASM)
target_include_directories(${SYSTEM_NAME} INTERFACE
    interrupts
    
    ports
)
target_sources(${SYSTEM_NAME} INTERFACE
	interrupts/interrupts.c
	
	ports/port_freertos.c
	ports/port_logger.c
)
target_link_directories(${SYSTEM_NAME} INTERFACE)
target_link_libraries(${SYSTEM_NAME} INTERFACE
	freertos
	driver_hal
)