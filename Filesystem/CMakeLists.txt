project(Filesystem)

include_directories(include)
include_directories(../Memory/include)
include_directories(../Exception/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src FS_SOURCE)

add_library(Filesystem SHARED ${FS_SOURCE})
target_include_directories(Filesystem PUBLIC
	String
)
target_link_libraries(Filesystem
	Memory
	String
	Exception
)