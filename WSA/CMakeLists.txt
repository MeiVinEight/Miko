project(WSA)

include_directories(
	include
	../Memory/include
	../String/include
	../Filesystem/include
	../Streaming/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
add_definitions(
	-DCMAKE_BUILD
	-DWSA_SHARED
)
aux_source_directory(src WSA_SOURCE)

add_library(WSA SHARED ${WSA_SOURCE})
target_link_libraries(WSA PUBLIC
	Streaming
	ws2_32
)
