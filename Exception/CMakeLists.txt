project(Exception)

include_directories(include)
include_directories(../Memory/include)
include_directories(../String/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src EXCEPTION_SOURCE)

add_library(Exception SHARED ${EXCEPTION_SOURCE})
target_link_libraries(Exception Memory)
target_link_libraries(Exception String)