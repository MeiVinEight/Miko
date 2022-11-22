project(Compression)

include_directories(include)
include_directories(../Memory/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src SOURCE)

add_library(Compression SHARED ${SOURCE})
target_link_libraries(Compression Memory)
