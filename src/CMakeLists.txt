

message(STATUS "11${PROJECT_SOURCE_DIR}")

add_subdirectory(decoder)
add_subdirectory(bitStream)
add_subdirectory(adtsHeader)
add_subdirectory(adtsData)

add_executable(AACDecoder main.cpp)

target_include_directories(AACDecoder
        PRIVATE ${PROJECT_SOURCE_DIR}/src/decoder
        )
target_link_libraries(AACDecoder
        PRIVATE decoder bitStream adtsHeader adtsData
        )