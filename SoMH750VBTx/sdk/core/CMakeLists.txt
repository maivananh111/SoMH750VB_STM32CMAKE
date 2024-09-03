cmake_minimum_required(VERSION 3.20)


set(CORE_NAME core)

project(${CORE_NAME})
add_library(${CORE_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${CORE_NAME} INTERFACE
	cpu
	mcu
	sysruntime
)
target_sources(${CORE_NAME} INTERFACE
	mcu/startup_stm32h750vbtx.s

	sysruntime/system_stm32h7xx.c
    sysruntime/sysmem.c
    sysruntime/syscalls.c
)
target_link_directories(${CORE_NAME} INTERFACE)