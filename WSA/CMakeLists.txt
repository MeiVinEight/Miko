project(WSA)

include_directories(include)
include_directories(../Memory/include)
include_directories(../Exception/include)
include_directories(../Filesystem/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src WSA_SOURCE)

add_library(WSA SHARED ${WSA_SOURCE})
target_include_directories(WSA PUBLIC
	Filesystem
)
target_link_libraries(WSA Exception)
target_link_libraries(WSA Filesystem)
