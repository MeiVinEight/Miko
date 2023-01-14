project(WST)

include_directories(
	include
	../Memory/include
	../Timestamp/include
	../String/include
	../Exception/include
	../Filesystem/include
	../WSA/include
	../CWS/include
	../HTTP/include
	../Compression/include
	../JSON/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src WST_SOURCE)

add_executable(WST ${WST_SOURCE})
target_link_libraries(WST
	Timestamp String Exception Filesystem WSA CWS HTTP Compression JSON
)