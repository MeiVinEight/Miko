project(WST)

include_directories(
	include
	../Memory/include
	../Timestamp/include
	../String/include
	../Filesystem/include
	../WSA/include
	../CWS/include
	../HTTP/include
	../Compression/include
	../JSON/include
	../Streaming/include
	../Cryptography/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src WST_SOURCE)

add_executable(WST ${WST_SOURCE})
target_link_libraries(WST
	Timestamp String Filesystem WSA CWS HTTP Compression JSON Streaming Cryptography
)