

message(STATUS "PROJECT_SOURCE_DIR${PROJECT_SOURCE_DIR}")
add_executable(AACDecoder main.cpp)
add_subdirectory(decoder)
add_subdirectory(bitStream)
add_subdirectory(adtsHeader)
add_subdirectory(adtsData)
add_subdirectory(ICS)
add_subdirectory(huffman)


target_include_directories(AACDecoder
        PRIVATE ${PROJECT_SOURCE_DIR}/src/decoder
        )
target_link_libraries(AACDecoder
        PRIVATE decoder bitStream adtsHeader adtsData ICS huffman
        )