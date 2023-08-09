cmake_minimum_required (VERSION 2.8)

project (demo)

set (TAGET ftp_client)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_definitions(-g)
add_definitions(-DNDEBUG -D__USE_FILE_OFFSET64)
aux_source_directory (./ TOP_SRC)
aux_source_directory (src SUB_SRC)

set (SRC_LIST 
	${TOP_SRC} 
	${SUB_SRC}
	)

include_directories (${PROJECT_SOURCE_DIR}
					 inc 
					 )

add_executable(${TAGET}
			${SRC_LIST}
			)

target_link_libraries(${TAGET}
		# pthread
		# m
		)
