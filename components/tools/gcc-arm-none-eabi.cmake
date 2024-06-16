set(CMAKE_SYSTEM_NAME               Generic)
set(CMAKE_SYSTEM_PROCESSOR          arm)

set(CMAKE_C_STANDARD 				11)
set(CMAKE_CXX_STANDARD 				14)
set(CMAKE_C_STANDARD_REQUIRED 		ON)
set(CMAKE_C_EXTENSIONS 				ON)
set(CMAKE_C_COMPILER_FORCED 		TRUE)
set(CMAKE_CXX_COMPILER_FORCED 		TRUE)
set(CMAKE_C_COMPILER_ID 			GNU)
set(CMAKE_CXX_COMPILER_ID 			GNU)
set(CMAKE_EXPORT_COMPILE_COMMANDS 	TRUE)

set(TOOLCHAIN_PREFIX                arm-none-eabi-)
set(CMAKE_C_COMPILER                ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER              ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_LINKER                    ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE                      ${TOOLCHAIN_PREFIX}size)

set(CMAKE_EXECUTABLE_SUFFIX_ASM     ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C       ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX     ".elf")

################# DO NOT CHANGE THIS VARIABLES #################
set(TARGET_FLAGS   					"-mcpu=cortex-m7 -mfpu=fpv5-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS 					"${CMAKE_C_FLAGS} ${TARGET_FLAGS} -Wall -Wextra -Wpedantic -fdata-sections -ffunction-sections")
if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_C_FLAGS 				"${CMAKE_C_FLAGS} -O0 -g3")
endif()
if(CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_C_FLAGS 				"${CMAKE_C_FLAGS} -Os -g0")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions -fno-threadsafe-statics")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp -MMD -MP")

set(CMAKE_C_LINK_FLAGS "${TARGET_FLAGS} -T\"${LINKER_FILE}\"")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} --specs=nano.specs")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,--start-group -lc -lm -Wl,--end-group -Wl,--print-memory-usage")

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -Wl,--start-group -lstdc++ -lsupc++ -Wl,--end-group")

################# CUSTOM FUNCTIONS #################

function(import_embedtxt_files build_target)
	
    set(NUM_FILES ${ARGC})
    math(EXPR NUM_FILES "${NUM_FILES} - 1")
    
	set(TOOLS_FILE_PATH "${CMAKE_SOURCE_DIR}/components/tools/gen_embedfile_src.py")
	message("** Generate tool: ${TOOLS_FILE_PATH}")

    foreach(idx RANGE 1 ${NUM_FILES})
        list(GET ARGV ${idx} file_path)
        
        set(INPUT_FILE_PATH "${file_path}")
        get_filename_component(INPUT_FILE ${INPUT_FILE_PATH} NAME)
        string(REPLACE "." "_" OUTPUT_FILE ${INPUT_FILE})
        set(VARIABLE_NAME			"_binary_${OUTPUT_FILE}")
        set(VARIABLE_LENGTH			"${VARIABLE_NAME}_len")
        set(OUTPUT_FILE      		"binary_${OUTPUT_FILE}.c")
        set(INPUT_FILE_PATH 		"${CMAKE_SOURCE_DIR}/${INPUT_FILE_PATH}")
        set(OUTPUT_FILE_PATH 		"${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_DIR}/${OUTPUT_FILE}")
        
		file(READ "${INPUT_FILE_PATH}" FILE_CONTENTS)
		string(LENGTH "${FILE_CONTENTS}" FILE_CONTENTS_LEN)
		math(EXPR FILE_CONTENTS_LEN "${FILE_CONTENTS_LEN} - 1")
		message("**** FILE_CONTENTS = ${FILE_CONTENTS}")
		
		file(READ "${INPUT_FILE_PATH}" HEX_CONTENTS HEX)
#		string(HEX ${FILE_CONTENTS} HEX_CONTENTS)
		string(LENGTH "${HEX_CONTENTS}" HEX_LENGTH)
		message("**** HEX_LENGTH = ${HEX_LENGTH}")
		math(EXPR EO_DATA "${HEX_LENGTH} - 4")
		set(HEX_ARRAY "")
	
		foreach(INDEX RANGE 0 ${EO_DATA} 2)
		    string(SUBSTRING "${HEX_CONTENTS}" ${INDEX} 2 BYTE)
		    
		    if(NOT "${BYTE}" STREQUAL "09")
		    	set(HEX_ARRAY "${HEX_ARRAY} 0x${BYTE},")
		    else()
		    	math(EXPR FILE_CONTENTS_LEN "${FILE_CONTENTS_LEN} - 1")
		    endif()
		    
		endforeach()
		string(REGEX REPLACE ",$" "" HEX_ARRAY "${HEX_ARRAY}")
		
		set(C_FILE_CONTENT 			"unsigned char ${VARIABLE_NAME}[] = {${HEX_ARRAY}};\r\nunsigned int ${VARIABLE_LENGTH} = ${FILE_CONTENTS_LEN};\n")
		
		file(WRITE "${OUTPUT_FILE_PATH}" "${C_FILE_CONTENT}")

    	target_sources(${build_target} INTERFACE ${OUTPUT_FILE_PATH})
    	
    endforeach()

endfunction()

#function(import_embedtxt_files build_target)
#	message("*****************import_embedtxt_files********************")
#	
#    set(NUM_FILES ${ARGC})
#    math(EXPR NUM_FILES "${NUM_FILES} - 1")
#    
#	set(TOOLS_FILE_PATH "${CMAKE_SOURCE_DIR}/components/tools/gen_embedfile_src.py")
#
#    foreach(idx RANGE 1 ${NUM_FILES})
#        list(GET ARGV ${idx} file_path)
#        
#        set(INPUT_FILE_PATH "${file_path}")
#        get_filename_component(INPUT_FILE ${INPUT_FILE_PATH} NAME)
#        string(REPLACE "." "_" OUTPUT_FILE ${INPUT_FILE})
#        set(OUTPUT_FILE      		"binary_${OUTPUT_FILE}.c")
#        set(INPUT_FILE_PATH 		"${CMAKE_SOURCE_DIR}/${INPUT_FILE_PATH}")
#        set(OUTPUT_FILE_PATH 		"${CMAKE_SOURCE_DIR}/${CMAKE_BUILD_DIR}/${OUTPUT_FILE}")
#        
#	    message("** ${INPUT_FILE_PATH} -> ${OUTPUT_FILE_PATH}")
#	    
#        add_custom_command(
#        	TARGET ${build_target}
#		    COMMAND python3 ${TOOLS_FILE_PATH} ${INPUT_FILE_PATH} ${OUTPUT_FILE_PATH}
#		    DEPENDS ${INPUT_FILE_PATH} ${TOOLS_FILE_PATH}
#		    COMMENT "** Converted ${INPUT_FILE_PATH} to ${CMAKE_BUILD_DIR}/${OUTPUT_FILE}"
#		)
#
#    endforeach()
#
#endfunction()





