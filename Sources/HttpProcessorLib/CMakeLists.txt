set(TARGET_NAME "HttpProcessorLib")
add_library(${TARGET_NAME} OBJECT)

target_sources(${TARGET_NAME} PRIVATE
    HttpProcessorUtils.cpp
    Processor.cpp
)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)


