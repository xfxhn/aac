add_library(adtsData OBJECT adtsData.cpp)


target_include_directories(adtsData
        PRIVATE ${PROJECT_SOURCE_DIR}/src/bitStream
        )
