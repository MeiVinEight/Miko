project(WTM)

include_directories(
	include
	../Memory/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
add_definitions(
	-DWTM_SHARED
	-DCMAKE_BUILD
)
aux_source_directory(src SOURCE)

add_library(WTM SHARED ${SOURCE})
target_link_libraries(WTM Memory)