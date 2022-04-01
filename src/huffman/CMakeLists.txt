add_library(huffman OBJECT huffman.cpp)


target_include_directories(huffman
        PRIVATE ${PROJECT_SOURCE_DIR}/src/bitStream
        )
