set(TARGET_NAME "processor")

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE
    main.cpp)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_link_libraries(${TARGET_NAME} PRIVATE HttpProcessorLib)

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})