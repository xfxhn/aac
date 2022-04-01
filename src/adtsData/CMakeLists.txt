add_library(adtsData OBJECT adtsData.cpp)


target_include_directories(adtsData
        PRIVATE ${PROJECT_SOURCE_DIR}/src/bitStream
        PRIVATE ${PROJECT_SOURCE_DIR}/src/ICS
        PRIVATE ${PROJECT_SOURCE_DIR}/src/adtsHeader
        PRIVATE ${PROJECT_SOURCE_DIR}/src/huffman
        )
