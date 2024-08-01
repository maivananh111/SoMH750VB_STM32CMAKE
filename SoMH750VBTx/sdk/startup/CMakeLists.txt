cmake_minimum_required(VERSION 3.20)


set(STARTUP_NAME startup)

project(${STARTUP_NAME})
add_library(${STARTUP_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${STARTUP_NAME} INTERFACE
	.
)
target_sources(${STARTUP_NAME} INTERFACE
	app_init.c
	app_hw_mspinit.c
	app_hw_init.c
)
target_link_directories(${STARTUP_NAME} INTERFACE)