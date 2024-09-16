cmake_minimum_required(VERSION 3.10)
project(Tests)

# Enable C++11 standard (or later)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-DENABLE_LOGGING_STDIO=1)

# Specify the source files for the project
set(SOURCES
  main.cpp
  ../app_layer_v1/byte_queue.cpp    
)

# Add the executable target
add_executable(Tests ${SOURCES})

target_include_directories(Tests PRIVATE ../)

