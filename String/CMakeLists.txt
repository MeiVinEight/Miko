project(String)

include_directories(
	include
	../Memory/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src STRING_SOURCE)

add_library(String SHARED ${STRING_SOURCE})
target_link_libraries(String PUBLIC Memory)
