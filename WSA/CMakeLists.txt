project(WSA)

include_directories(include)
include_directories(../Memory/include)
include_directories(../Exception/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src WSA_SOURCE)

add_library(WSA SHARED ${WSA_SOURCE})
target_link_libraries(WSA Memory)
target_link_libraries(WSA Exception)
