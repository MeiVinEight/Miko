project(CWS)

include_directories(include)
include_directories(../Memory/include)
include_directories(../String/include)
include_directories(../Exception/include)
include_directories(../Filesystem/include)
include_directories(../WSA/include)
include_directories(../HTTP/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src CWS_SOURCE)

add_library(CWS SHARED ${CWS_SOURCE})
target_link_libraries(CWS WSA)
target_link_libraries(CWS HTTP)
target_link_libraries(CWS Exception)
