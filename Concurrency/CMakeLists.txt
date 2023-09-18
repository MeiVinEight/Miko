project(Concurrency)

include_directories(
	include
	../Memory/include
	../WTM/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
add_definitions(
	-DCMAKE_BUILD
	-DCONCURRENCY_SHARED
)
aux_source_directory(src SOURCE)

add_library(Concurrency SHARED ${SOURCE})
target_link_libraries(Concurrency PUBLIC
	Memory
	WTM
)