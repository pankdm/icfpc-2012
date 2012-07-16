include_directories (${MINING_SOURCE_DIR}/lib)
link_directories (${MINING_BINARY_DIR}/lib)

add_executable (sandbox main.cpp)

target_link_libraries (sandbox lib) 