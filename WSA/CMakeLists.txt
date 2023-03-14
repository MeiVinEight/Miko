project(WSA)

include_directories(
	include
	../Memory/include
	../Exception/include
	../Filesystem/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src WSA_SOURCE)

add_library(WSA SHARED ${WSA_SOURCE})
target_link_libraries(WSA PUBLIC
	Exception
	Filesystem
)
