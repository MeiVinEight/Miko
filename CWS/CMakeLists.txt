project(CWS)

include_directories(
	include
	../Memory/include
	../String/include
	../Streaming/include
	../WSA/include
	../Cryptography/include
	../Timestamp/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src CWS_SOURCE)

add_library(CWS SHARED ${CWS_SOURCE})
target_link_libraries(CWS PUBLIC
	WSA
	Cryptography
	Timestamp
	String
)
