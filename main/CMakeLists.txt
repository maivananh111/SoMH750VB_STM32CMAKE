cmake_minimum_required(VERSION 3.22)


set(PROJECT_NAME main)

project(${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${PROJECT_NAME} INTERFACE
	.
)

target_sources(${PROJECT_NAME} INTERFACE
	app_init.c
    app_main.c
)

target_link_libraries(${PROJECT_NAME} INTERFACE
	components_drivers
)

target_link_directories(${PROJECT_NAME} INTERFACE 

)











