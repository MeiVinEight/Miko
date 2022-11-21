project(HTTP)

include_directories(include)
include_directories(../Memory/include)
include_directories(../String/include)
include_directories(../Exception/include)
include_directories(../Filesystem/include)
include_directories(../WSA/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src HTTP_SOURCE)

add_library(HTTP SHARED ${HTTP_SOURCE})
target_link_libraries(HTTP String)
target_link_libraries(HTTP Exception)
target_link_libraries(HTTP WSA)