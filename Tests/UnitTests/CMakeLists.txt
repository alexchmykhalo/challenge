set(TARGET_NAME "HttpProcessorLibTests")

add_executable(${TARGET_NAME})
target_sources(${TARGET_NAME} PRIVATE
    HttpProcessorLibTests.cpp)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

# Add Catch2 unit testing lib
Include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.3
)
FetchContent_MakeAvailable(Catch2)
target_link_libraries(${TARGET_NAME} PRIVATE Catch2::Catch2WithMain)

# Add automatic test discovery
include(CTest)
include(Catch)
catch_discover_tests(${TARGET_NAME})