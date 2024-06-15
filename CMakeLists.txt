cmake_minimum_required(VERSION 3.20)

############ CHANGE BUILD TARGET NAME ############
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(CONFIG_H_IN_FILE            "tools/config.h.in")
set(CONFIG_H_FILE               "config.h")
set(CMAKE_TOOLCHAIN_FILE 		"tools/gcc-arm-none-eabi.cmake")
set(LINKER_FILE    				"${CMAKE_SOURCE_DIR}/components/system/STM32H750VBTX_FLASH.ld")

set(MACROS_DEFINATION
	USE_HAL_DRIVER 
	STM32H750xx
    $<$<CONFIG:Debug>:DEBUG>
) 

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE 		"Debug")
endif()

message("***Project       : ${PROJECT_NAME}")
message("***Build type    : ${CMAKE_BUILD_TYPE}")
message("***Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
message("***Linker file   : ${LINKER_FILE}")
message("***Binary dir    : ${CMAKE_BUILD_TYPE}")

include(${CMAKE_TOOLCHAIN_FILE})

################## PROJECT SETUP #################
enable_language(C CXX ASM)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME})

add_compile_definitions(${MACROS_DEFINATION})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>)
configure_file(${CONFIG_H_IN_FILE} ${CONFIG_H_FILE})

################# LIBRARIES LINK #################
add_subdirectory(main)
add_subdirectory(components)

target_link_libraries(${PROJECT_NAME}
    main
    components_cmsis
    components_device
    components_hal
    components_system
	components_freertos
)

include_directories(${CMAKE_BUILD_TYPE})


################# DO NOT CHANGE THIS VARIABLES #################
set(TARGET_FLAGS   					"-mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS 					"${CMAKE_C_FLAGS} ${TARGET_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS 				"${CMAKE_C_FLAGS} -O0 -g3")
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS 				"${CMAKE_C_FLAGS} -Os -g0")
endif()
message("***CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")

set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS} -T\"${LINKER_FILE}\"")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")


if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "***Generated code requires C11 or higher")
endif()








