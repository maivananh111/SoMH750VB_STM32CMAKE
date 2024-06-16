cmake_minimum_required(VERSION 3.20)

find_package(Python 3 REQUIRED)
############ CHANGE BUILD TARGET NAME ############
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(BUILD_TARGET                ${PROJECT_NAME})
set(CMAKE_BUILD_DIR             "build")
set(CMAKE_TOOLCHAIN_FILE 		"components/tools/gcc-arm-none-eabi.cmake")
set(CONFIG_H_IN_FILE            "components/tools/config.h.in")
set(CONFIG_H_FILE               "config.h")
set(LINKER_FILE    				"${CMAKE_SOURCE_DIR}/components/system/STM32H750VBTX_FLASH.ld")

set(MACROS_DEFINATION
	USE_HAL_DRIVER 
	STM32H750xx
    $<$<CONFIG:Debug>:DEBUG>
) 

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE 		"Debug")
endif()

message("***************** Projects ********************")
message("** Project       : ${PROJECT_NAME}")
message("** Build type    : ${CMAKE_BUILD_TYPE}")
message("** Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
message("** Linker file   : ${LINKER_FILE}")
message("** Binary dir    : ${CMAKE_BUILD_DIR}")

include(${CMAKE_TOOLCHAIN_FILE})


################## [DO NOT CHANGE IT] PROJECT SETUP #################
enable_language(C CXX ASM)

project(${PROJECT_NAME})
add_executable(${BUILD_TARGET})

add_compile_definitions(${MACROS_DEFINATION})
add_custom_command(TARGET ${BUILD_TARGET} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${BUILD_TARGET}>)
configure_file(${CONFIG_H_IN_FILE} ${CONFIG_H_FILE})

################# LIBRARIES LINK #################
add_subdirectory(main)
add_subdirectory(components)

target_link_libraries(${BUILD_TARGET} PUBLIC
    main
    components_cmsis
    components_device
    components_hal
    components_system
	components_freertos
)

include_directories(${CMAKE_BUILD_DIR}
	${CMAKE_BUILD_DIR}
)









