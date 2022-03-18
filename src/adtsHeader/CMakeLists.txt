add_library(adtsHeader OBJECT adtsHeader.cpp)


target_include_directories(adtsHeader
        PRIVATE ${PROJECT_SOURCE_DIR}/src/bitStream
        )