set(PROJECT_NAME main)

project(${PROJECT_NAME})
add_library(${PROJECT_NAME} INTERFACE)
enable_language(C CXX ASM)


import_embedtxt_files(${PROJECT_NAME}
	main/files/xyz.txt
)

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









