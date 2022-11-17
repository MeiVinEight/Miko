project(CWS)

include_directories(include)
include_directories(../Memory/include)
include_directories(../FileSystem/include)
include_directories(../WSA/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src CWS_SOURCE)

add_library(CWS SHARED ${CWS_SOURCE})
target_link_libraries(CWS WSA)
