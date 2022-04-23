##########################################################################
#                             Configuration                              #
##########################################################################

cmake_minimum_required(VERSION 3.0.0)
project(ocr-receipt VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(EXTRA_COMPILE_FLAGS "-Wall -Werror")
set(BINARY ${CMAKE_PROJECT_NAME})
set(TEST ${CMAKE_PROJECT_NAME}-test)

find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem system)
include_directories(${Boost_INCLUDE_DIR})

find_package(Leptonica 1.82.0 REQUIRED)
include_directories(${Leptonica_INCLUDE_DIRS})

find_package(Tesseract 5.1.0 REQUIRED)
include_directories(${Tesseract_INCLUDE_DIRS})

##########################################################################
#                                 Binary                                 #
##########################################################################

include_directories(include)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${BINARY} ${SOURCES} ${INCLUDES} main.cpp)
target_link_libraries(${BINARY} ${Boost_LIBRARIES} ${Tesseract_LIBRARIES} ${Leptonica_LIBRARIES})

##########################################################################
#                               Unit Tests                               #
##########################################################################

enable_testing()

include(GoogleTest)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip
)
FetchContent_MakeAvailable(googletest)

set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
file(GLOB_RECURSE SOURCES_TEST ${TEST_DIR}/*.cpp)

add_executable(${TEST} ${SOURCES} ${SOURCES_TEST} ${INCLUDES})
target_link_libraries(${TEST} gtest_main ${Boost_LIBRARIES} ${Tesseract_LIBRARIES} ${Leptonica_LIBRARIES})

gtest_discover_tests(${TEST})
