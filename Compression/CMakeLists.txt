project(Compression)

include_directories(
	include
	../Memory/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
add_definitions(
	-DCMAKE_BUILD
	-DCOMPRESSION_SHARED
)
aux_source_directory(src SOURCE)

add_library(Compression SHARED ${SOURCE})
target_link_libraries(Compression PUBLIC
	Memory
)
