cmake_minimum_required(VERSION 3.10)
project(Tests)

# Enable C++11 standard (or later)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the source files for the project
set(SOURCES
  main.cpp
  ../app_layer_v1/byte_queue.cpp    
)

# Add the executable target
add_executable(Tests ${SOURCES})

# If you need to add specific libraries or include directories, do it here
# For example, you can use the `target_include_directories` or `target_link_libraries` commands if needed
# Example:
# target_include_directories(Tests PRIVATE ${SOME_INCLUDE_DIR})
# target_link_libraries(Tests ${SOME_LIBRARIES})
