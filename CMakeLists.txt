cmake_minimum_required(VERSION 3.0.0)
project(ocr-receipt VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(EXTRA_COMPILE_FLAGS "-Wall -Werror")
set(BINARY ${CMAKE_PROJECT_NAME}-debug)

find_package(OpenCV 4.1.0 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(Boost 1.67.0 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(Tesseract 3.05 REQUIRED)
include_directories(${Tesseract_INCLUDE_DIRS})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${BINARY} ${SOURCES} ${INCLUDES} main.cpp)
target_link_libraries(${BINARY} ${OpenCV_LIBS} ${Boost_LIBRARIES} ${Tesseract_LIBRARIES})
