##########################################################################
#                             Configuration                              #
##########################################################################

cmake_minimum_required(VERSION 3.2...3.5)
project(ocr-receipt VERSION 1.0.0)
if(POLICY CMP0135)
	cmake_policy(SET CMP0135 NEW)
	set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(EXTRA_COMPILE_FLAGS "-Wall -Werror")
set(BINARY ${CMAKE_PROJECT_NAME})
set(TEST ${CMAKE_PROJECT_NAME}-test)
option(DOCKERLESS_BUILD "Loads a different set of configurations  and build processes depending on wether or not docker is going to be used" OFF)


find_package(Boost 1.74.0 REQUIRED COMPONENTS filesystem locale program_options system)
include_directories(${Boost_INCLUDE_DIR})

find_package(OpenCV 4.5.4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(PythonLibs 3.10.4 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

############################################################
#        Handle Leptonica and Tesseract Libraries          #
#        if they are found we carry else we need to        #
#        build them statically at configure time so        #
#        that they are accessible at build time            #
#        this is handled by a seperate cmake file          #
#        which is included only if one of the libraries    #
#        is non existent or the DOCKERLESS_BUILD Option    #
#        is set to ON.                                     #
############################################################
find_package(Leptonica 1.82.0 QUIET) # DO NOT SET THIS TO REQUIRED
find_package(Tesseract 5.1.0 QUIET)  # DO NOT SET THIS TO REQUIRED

if(NOT Leptonica_FOUND OR NOT Tesseract_FOUND OR DOCKERLESS_BUILD)
	message(STATUS "Building in Docker-less Mode")
	include(TesseractLeptExternalStatics.cmake)
else ()
	include_directories(${Leptonica_INCLUDE_DIRS})
	include_directories(${Tesseract_INCLUDE_DIRS})
endif ()




add_definitions(-DBOOST_BIND_GLOBAL_PLACEHOLDERS)

##########################################################################
#                                 Binary                                 #
##########################################################################

include_directories(include)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(${BINARY} ${SOURCES} ${INCLUDES} main.cpp)
if(EXTERNAL_STATIC_BUILD_TRIGGERED)


	target_link_libraries(${BINARY} ${Boost_LIBRARIES} ${OpenCV_LIBS} ${PYTHON_LIBRARIES}  ${EXTRA_IMAGES_DYNAMIC_LIBRARIES} ${Tesseract_STATICLIB_TARGET} ${Leptonica_STATICLIB_TARGET})

	if (DEFINED TESSDATA_DIR)
		message(WARNING "    ==================================================================================================\n"
				"    TESSDATA_PREFIX env Variable must be set to ${TESSDATA_DIR}\n"
				"    Export this variable manually or set it persistently in a .bashrc or equivalent.\n"
				"    ==================================================================================================\n"
		)
	endif ()

else ()
	target_link_libraries(${BINARY} ${Boost_LIBRARIES} ${Tesseract_LIBRARIES} ${Leptonica_LIBRARIES} ${OpenCV_LIBS} ${PYTHON_LIBRARIES})
endif ()



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
if(EXTERNAL_STATIC_BUILD_TRIGGERED)
	target_link_libraries(${TEST} gtest_main ${Boost_LIBRARIES} ${OpenCV_LIBS} ${PYTHON_LIBRARIES} ${EXTRA_IMAGES_DYNAMIC_LIBRARIES} ${Tesseract_STATICLIB_TARGET} ${Leptonica_STATICLIB_TARGET})
else ()
	target_link_libraries(${TEST} gtest_main ${Boost_LIBRARIES} ${Tesseract_LIBRARIES} ${Leptonica_LIBRARIES} ${OpenCV_LIBS} ${PYTHON_LIBRARIES})
endif ()
gtest_discover_tests(${TEST})
