cmake_minimum_required(VERSION 3.20)


set(DRIVER_NAME driver_periph)
file(GLOB PERIPH_SOURCE_FILES "source/*.c")


project(${DRIVER_NAME})
add_library(${DRIVER_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${DRIVER_NAME} INTERFACE
    include
)
target_sources(${DRIVER_NAME} INTERFACE
	${PERIPH_SOURCE_FILES}
)
target_link_directories(${DRIVER_NAME} INTERFACE)
target_link_libraries(${DRIVER_NAME} INTERFACE 
	driver_hal
	freertos	
)