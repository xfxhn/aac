

add_library(ICS OBJECT ICS.cpp)


target_include_directories(ICS
        PRIVATE ${PROJECT_SOURCE_DIR}/src/bitStream
        PRIVATE ${PROJECT_SOURCE_DIR}/src/adtsHeader
        )