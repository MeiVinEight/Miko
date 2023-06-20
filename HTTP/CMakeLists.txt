project(HTTP)

include_directories(
	include
	../Memory/include
	../String/include
	../Filesystem/include
	../Streaming/include
	../WSA/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
add_definitions(
	-DHTTP_SHARED
	-DCMAKE_BUILD
)
aux_source_directory(src HTTP_SOURCE)

add_library(HTTP SHARED ${HTTP_SOURCE})
target_link_libraries(HTTP PUBLIC
	String
	Streaming
)