cmake_minimum_required(VERSION 3.20)


set(PROJECT_NAME components_drivers)
file(GLOB DRIVER_SOURCE_FILES "source/*.c")


project(${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${PROJECT_NAME} INTERFACE
    include
)
target_sources(${PROJECT_NAME} INTERFACE
	${DRIVER_SOURCE_FILES}
)
target_link_libraries(${PROJECT_NAME} INTERFACE
	components_freertos
)
target_link_directories(${PROJECT_NAME} INTERFACE

)