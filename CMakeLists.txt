cmake_minimum_required(VERSION 3.13)
project(VL1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)
find_package(glm CONFIG REQUIRED)

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


## GOOGLE TEST
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCE "src/*.cpp")

foreach(src_file ${SOURCE})
    if(src_file MATCHES "/test/")
        message(${src_file}}\n)
        list(REMOVE_ITEM SOURCE ${src_file})
    endif()
endforeach()

file(GLOB INCLUDE_DIRS RELATIVE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/include/*)
list(REMOVE_ITEM INCLUDE_DIRS "ts")

include_directories(${CMAKE_SOURCE_DIR}/include)

foreach(dir ${INCLUDE_DIRS})
    include_directories(${CMAKE_SOURCE_DIR}/include/${dir})
endforeach()

add_executable(VL1 ${SOURCE})

target_include_directories(VL1 PRIVATE 
    ${GLFW_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(VL1 PRIVATE
    ${GLFW_LIBRARIES}
    ${OpenCV_LIBS}
    OpenGL::GL
    glm::glm
    pthread
)

target_link_options(VL1 PRIVATE ${GLFW_LDFLAGS})

set(TEST_SOURCE
    src/serial/test/test_serial_linux.cpp
    src/serial/serial_linux.cpp
)

add_executable(runUnitTests ${TEST_SOURCE})

target_include_directories(runUnitTests PRIVATE 
    ${PROJECT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(runUnitTests PRIVATE
    GTest::gtest_main
    pthread
)

enable_testing()
add_test(NAME SERIAL_TEST COMMAND runUnitTests)

