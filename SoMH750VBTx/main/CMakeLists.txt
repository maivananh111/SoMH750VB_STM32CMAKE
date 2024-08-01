set(PROJECT_NAME main)

project(${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
enable_language(C CXX ASM)


import_embedtxt_files(${PROJECT_NAME} DATA_IN_EFLASH
	main/files/xyz.txt
)

target_include_directories(${PROJECT_NAME} INTERFACE
	.
)

target_sources(${PROJECT_NAME} INTERFACE
    app_main.c
)

target_link_libraries(${PROJECT_NAME} INTERFACE
	driver_hal
)

target_link_directories(${PROJECT_NAME} INTERFACE 

)









