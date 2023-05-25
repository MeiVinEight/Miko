project(compile)

include_directories(
	../Memory/include
	../String/include
	../JSON/include
	../Filesystem/include
	../Streaming/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src SRC)

add_executable(compile ${SRC})
target_link_libraries(compile Memory String JSON Filesystem Streaming)