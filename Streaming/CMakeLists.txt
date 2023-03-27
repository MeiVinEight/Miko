project(Streaming)

include_directories(
	include
	../Memory/include
	../String/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src SOURCE)

add_library(Streaming SHARED ${SOURCE})
target_link_libraries(Streaming PUBLIC
	Memory String
)