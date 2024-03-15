cmake_minimum_required(VERSION 3.23)
project(FmuGenerationUtility VERSION 1.0 DESCRIPTION "Utility to generate FMU for the given Cpp Component" LANGUAGES C CXX)

add_subdirectory(Sources)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(Tests)
endif()