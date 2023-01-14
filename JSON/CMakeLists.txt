project(JSON)

include_directories(
	include
	../Memory/include
	../String/include
	../Exception/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src SRC)

add_library(JSON SHARED ${SRC})
target_link_libraries(JSON Memory String Exception)