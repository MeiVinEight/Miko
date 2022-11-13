project(WST)

include_directories(include)
include_directories(../Memory/include)
include_directories(../String/include)
include_directories(../WSA/include)
include_directories(../CWS/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src WST_SOURCE)

add_executable(WST ${WST_SOURCE})
target_link_libraries(WST WSA)
target_link_libraries(WST CWS)
target_link_libraries(WST String)
