project(Filesystem)

include_directories(
	include
	../Memory/include
	../Exception/include
	../String/include
	../Streaming/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src FS_SOURCE)

add_library(Filesystem SHARED ${FS_SOURCE})
target_link_libraries(Filesystem PUBLIC
	Memory
	String
	Exception
	Streaming
)