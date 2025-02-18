cmake_minimum_required(VERSION 3.13)
project(VL1)


set(SOURCE
    src/main.cpp
    src/loadsave.cpp
    src/window_io.cpp
    src/lidar_base.cpp
    src/LidarDataParser.cpp
    src/YDLidarController.cpp
    src/lidar_tminiplus.cpp
    
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(VL1 ${SOURCE})


target_include_directories(VL1 PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(VL1)