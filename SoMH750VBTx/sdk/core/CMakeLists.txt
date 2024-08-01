cmake_minimum_required(VERSION 3.20)


set(CORE_NAME core)

project(${CORE_NAME})
add_library(${CORE_NAME} INTERFACE)
enable_language(C CXX ASM)

target_include_directories(${CORE_NAME} INTERFACE
	cpu
	mcudev
	mcusys
)
target_sources(${CORE_NAME} INTERFACE
	mcudev/startup_stm32h750vbtx.s

	mcusys/system_stm32h7xx.c
    mcusys/sysmem.c
    mcusys/syscalls.c
)
target_link_directories(${CORE_NAME} INTERFACE)