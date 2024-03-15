cmake_minimum_required(VERSION 3.28)
project(Processor VERSION 1.0 DESCRIPTION "Utility to write statistics about Http requests/responses read from the Unix pipe." LANGUAGES C CXX)

add_subdirectory(HttpProcessorLib)
add_subdirectory(Processor)