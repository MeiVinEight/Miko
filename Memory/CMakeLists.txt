project(Memory)

include_directories(include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src MEMORY_SOURCE)

add_library(Memory SHARED ${MEMORY_SOURCE})