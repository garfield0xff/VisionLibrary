cmake_minimum_required(VERSION 3.13)
project(VL1)

set(SOURCE
    src/main.cpp
)

add_executable(VL1 ${SOURCE})


target_include_directories(VL1 PRIVATE )
target_link_libraries(VL1)