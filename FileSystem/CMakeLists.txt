project(FileSystem)

include_directories(include)
include_directories(../Memory/include)
include_directories(../Exception/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
aux_source_directory(src FS_SOURCE)

add_library(FileSystem SHARED ${FS_SOURCE})
target_link_libraries(FileSystem Memory)
target_link_libraries(FileSystem Exception)