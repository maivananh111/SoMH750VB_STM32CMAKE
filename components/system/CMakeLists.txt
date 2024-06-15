cmake_minimum_required(VERSION 3.20)



set(PROJECT_NAME components_system)

project(${PROJECT_NAME})
enable_language(C ASM)


add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE
    .
)
target_sources(${PROJECT_NAME} INTERFACE
	interrupts.c
	system_stm32h7xx.c
    sysmem.c
    syscalls.c
    startup_stm32h750vbtx.s
)
target_link_directories(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME} INTERFACE)














