


add_library(decoder OBJECT decoder.cpp)


target_include_directories(decoder
        PRIVATE ${PROJECT_SOURCE_DIR}/src/bitStream
        PRIVATE ${PROJECT_SOURCE_DIR}/src/adtsHeader
        PRIVATE ${PROJECT_SOURCE_DIR}/src/adtsData
        )