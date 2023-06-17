project(Filesystem)

include_directories(
	include
	../Memory/include
	../String/include
	../Streaming/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
add_definitions(
	-DCMAKE_BUILD
	-DFS_SHARED
)
aux_source_directory(src FS_SOURCE)

add_library(Filesystem SHARED ${FS_SOURCE})
target_link_libraries(Filesystem PUBLIC
	Memory
	String
	Streaming
)