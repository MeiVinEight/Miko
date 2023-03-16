project(Timestamp)

include_directories(
	include
	../Memory/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src SOURCE)


add_library(Timestamp SHARED ${SOURCE})
target_link_libraries(Timestamp)