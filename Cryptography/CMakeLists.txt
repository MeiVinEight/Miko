project(Cryptography)

include_directories(
	include
	../Memory/include
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Miko)
add_definitions(
	-DCMAKE_BUILD
	-DCRYPTO_SHARED
)
aux_source_directory(src SOURCE)

add_library(Cryptography SHARED ${SOURCE})
target_link_libraries(Cryptography PUBLIC Memory bcrypt)