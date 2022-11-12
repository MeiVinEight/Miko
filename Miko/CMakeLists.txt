project(Miko)

include_directories(include)
include_directories(../WSA/include)
include_directories(../CWS/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src MIKO_SOURCE)

add_executable(Miko ${MIKO_SOURCE})
target_link_libraries(Miko WSA)
target_link_libraries(Miko CWS)
