cmake_minimum_required(VERSION 3.20)


set(FREERTOS_NAME freertos)
file(GLOB FREERTOS_SOURCE_FILES "source/*.c")


project(${FREERTOS_NAME})
add_library(${FREERTOS_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${FREERTOS_NAME} INTERFACE
    include
)
target_sources(${FREERTOS_NAME} INTERFACE
	${FREERTOS_SOURCE_FILES}
)
target_link_directories(${FREERTOS_NAME} INTERFACE)
target_link_libraries(${FREERTOS_NAME} INTERFACE)