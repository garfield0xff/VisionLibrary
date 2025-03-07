cmake_minimum_required(VERSION 3.13)
project(VL1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Doxygen REQUIRED)
# if(Doxygen_FOUND)
#     set(DOXYGEN_IN      ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
#     set(DOXYGEN_OUT     ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#     configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

#     add_custom_target( doc ALL
#         COMMAND ${DOXYGEN_EXECUTABLE}   ${DOXYGEN_OUT}
#         WORKING_DIRECTORY   ${CMAKE_CURRENT_SOURCE_DIR}
#         COMMENT "Generating API DOCUMENT with DOXYGEN"
#         VERBATIM
#     )

# endif()

# set(CMAKE_DOXYGEN_LAYOUT "${CMAKE_CURRENT_BINARY_DIR}/docs/layout.xml")

find_package(OpenGL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

find_package(glm CONFIG REQUIRED)
message(STATUS "Found GLM version ${glm_VERSION}")


set(SOURCE
    src/main.cpp
    src/loadsave.cpp
    src/window_io.cpp
    src/lidar_base.cpp
    src/lidar_res.cpp
    src/lidar_tminiplus.cpp
)

add_executable(VL1 ${SOURCE})


target_include_directories(VL1 PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(VL1 PRIVATE
    ${GLFW_LIBRARIES}
    ${OpenCV_LIBS}
    OpenGL::GL
    glm::glm
)

target_link_options(VL1 PRIVATE ${GLFW_LDFLAGS})