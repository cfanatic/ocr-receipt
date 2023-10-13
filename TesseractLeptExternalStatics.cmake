include(FetchContent)
include(ExternalProject)
include(ProcessorCount)


set(EXTERNAL_STATIC_BUILD_TRIGGERED ON CACHE BOOL "" FORCE)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
    set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
endif()

#######################################################################
#              Set Path Variables                                     #
#######################################################################
set(EXTERNAL_LIBS_DIR "${CMAKE_SOURCE_DIR}/External")
set(LEPTONICA_SRC_DIR "${EXTERNAL_LIBS_DIR}/leptonica")
set(TESSERACT_SRC_DIR "${EXTERNAL_LIBS_DIR}/tesseract")
set(LOCAL_FAKEROOT_DIR "${CMAKE_SOURCE_DIR}/local_root")
set(LOCAL_FAKEROOT_INSTALL_PREFIX "${LOCAL_FAKEROOT_DIR}/usr")
set(LOCAL_FAKEROOT_INSTALL_INCLUDE_DIR "${LOCAL_FAKEROOT_INSTALL_PREFIX}/include")
set(LOCAL_FAKEROOT_INSTALL_SHARE_DIR "${LOCAL_FAKEROOT_INSTALL_PREFIX}/local/share")
set(LOCAL_FAKEROOT_INSTALL_LIBRARY_DIR "${LOCAL_FAKEROOT_INSTALL_PREFIX}/lib")
set(TESSDATA_DIR "${LOCAL_FAKEROOT_INSTALL_SHARE_DIR}/tessdata")

include_directories(${LOCAL_FAKEROOT_INSTALL_INCLUDE_DIR})

#######################################################################
#              We need  CURL and LibArchive                           #
#######################################################################
find_package(CURL)
find_package(LibArchive)
##################################################################################
#                                                                                #
#        Section Imported from Leptonica CMake . we need it to link              #
#        Tesseract to leptonica statically without issues                        #
#                                                                                #
#                                                                                #
##################################################################################
find_package(GIF)
find_package(JPEG)
find_package(PNG)
find_package(TIFF)
find_package(ZLIB)
find_package(PkgConfig)
if (PKG_CONFIG_FOUND)
    pkg_check_modules(WEBP libwebp QUIET)
    pkg_check_modules(WEBPMUX libwebpmux>=${MINIMUM_WEBPMUX_VERSION} QUIET)
    pkg_check_modules(JP2K libopenjp2>=2.0 QUIET)
endif()
if(NOT WEBP)
    find_path(WEBP_INCLUDE_DIR /webp/decode.h)
    find_library(WEBP_LIBRARY NAMES webp)
    if (WEBP_INCLUDE_DIR AND WEBP_LIBRARY)
        set(WEBP 1)
        set(WEBP_FOUND TRUE)
        set(WEBP_LIBRARIES ${WEBP_LIBRARY})
        set(WEBP_INCLUDE_DIRS ${WEBP_INCLUDE_DIR})
    endif()
endif()
if(NOT WEBPMUX)
    find_path(WEBPMUX_INCLUDE_DIR /webp/mux.h)
    #TODO:  check minimal required version
    if(NOT WEBPMUX_INCLUDE_DIR)
        message(STATUS "Can not find: /webp/mux.h")
    endif()
    if(NOT "${WEBPMUX_INCLUDE_DIR}" STREQUAL "${WEBP_INCLUDE_DIR}")
        set(WEBP_INCLUDE_DIRS ${WEBP_INCLUDE_DIRS} ${WEBPMUX_INCLUDE_DIR})
    endif()
    find_library(WEBPMUX_LIBRARY NAMES webpmux)
    if (WEBPMUX_INCLUDE_DIR AND WEBPMUX_LIBRARY)
        set(WEBPMUX 1)
        set(HAVE_LIBWEBP_ANIM 1)
        set(WEBPMUX_FOUND TRUE)
        set(WEBP_LIBRARIES ${WEBP_LIBRARIES} ${WEBPMUX_LIBRARY})
    endif()
endif()
if(NOT JP2K)
    find_path(JP2K_INCLUDE_DIR /openjpeg-2.3/openjpeg.h)
    find_library(JP2K_LIBRARY NAMES openjp2)
    if (JP2K_INCLUDE_DIR AND JP2K_LIBRARY)
        set(JP2K 1)
        set(JP2K_FOUND TRUE)
        set(JP2K_LIBRARIES ${JP2K_LIBRARY})
        set(JP2K_INCLUDE_DIRS ${JP2K_INCLUDE_DIR})
        set(HAVE_LIBJP2K 1)
    endif()
endif()
if(NOT JP2K)
    find_path(JP2K_INCLUDE_DIR /openjpeg-2.4/openjpeg.h)
    find_library(JP2K_LIBRARY NAMES openjp2)
    if (JP2K_INCLUDE_DIR AND JP2K_LIBRARY)
        set(JP2K 1)
        set(JP2K_FOUND TRUE)
        set(JP2K_LIBRARIES ${JP2K_LIBRARY})
        set(JP2K_INCLUDE_DIRS ${JP2K_INCLUDE_DIR})
        set(HAVE_LIBJP2K 1)
    endif()
endif()

#######################################################################
#              END OF FOREIGN LEPTONICA SECTION IMPORT                #
#######################################################################

#######################################################################
#              Set Dynamic Libraries we need to link against          #
#######################################################################
set(EXTRA_IMAGES_DYNAMIC_LIBRARIES ${JPEG_LIBRARIES}
        ${PNG_LIBRARIES}
        ${CURL_LIBRARIES}
        ${WEBP_LIBRARIES}
        ${ZLIB_LIBRARIES}
        ${TIFF_LIBRARIES}
        ${JP2K_LIBRARIES}
        ${GIF_LIBRARIES}
        ${LibArchive_LIBRARIES}
        ${CURL_LIBRARIES}
)



##########################################################################
#                           Prepare For Static Build                     #
##########################################################################

cmake_host_system_information(RESULT N_CORES QUERY NUMBER_OF_LOGICAL_CORES)
if(N_CORES EQUAL 0)
    message(WARNING "Unable to query core numbers. setting to 1")
    set(N_PROC 1)
elseif (N_CORES GREATER 1)
    math(EXPR N_PROC "${N_CORES} - 1" OUTPUT_FORMAT DECIMAL)
else ()
    set(N_PROC ${N_CORES})
endif ()

message(STATUS "${N_CORES} Logical Cores Available using ${N_PROC}")

##########################################################################
#                          Leptonica External Static Build               #
##########################################################################
FetchContent_Populate(leptonicaPopulate
        GIT_REPOSITORY https://github.com/DanBloomberg/leptonica.git
        GIT_TAG        1.82.0
        SOURCE_DIR "${LEPTONICA_SRC_DIR}"
)


set(LEPTONICA_BUILD_ARTIFACTS_DIR "${LEPTONICA_SRC_DIR}/build")
file(MAKE_DIRECTORY ${LEPTONICA_BUILD_ARTIFACTS_DIR})

if(NOT LEPTONICA_LIB_BUILD)
    message(STATUS "Leptonica library not built. Building")
    execute_process(COMMAND ${CMAKE_COMMAND} ".." "-DCMAKE_INSTALL_PREFIX='${LOCAL_FAKEROOT_INSTALL_PREFIX}'"
            WORKING_DIRECTORY "${LEPTONICA_BUILD_ARTIFACTS_DIR}")
    execute_process(COMMAND ${CMAKE_COMMAND}  "--build" "." "--parallel" "${N_PROC}"
            WORKING_DIRECTORY "${LEPTONICA_BUILD_ARTIFACTS_DIR}")
    execute_process(COMMAND ${CMAKE_COMMAND} "--install" "."
            WORKING_DIRECTORY "${LEPTONICA_BUILD_ARTIFACTS_DIR}"
            RESULT_VARIABLE LEPTONICA_BUILD_RESULT)

    if(${LEPTONICA_BUILD_RESULT} EQUAL 0)
        message(STATUS "Built leptonica successfully")
        set(LEPTONICA_LIB_BUILD ON CACHE BOOL "" FORCE)
    endif ()
else ()
    message(STATUS "Leptonica library built. Skipping")
endif ()




set(leptonica_INCLUDE_DIRS  "${LOCAL_FAKEROOT_INSTALL_INCLUDE_DIR}/leptonica")
set(leptonica_DIR  "${LOCAL_FAKEROOT_INSTALL_LIBRARY_DIR}/cmake/leptonica")
include_directories(${leptonica_INCLUDE_DIRS} ${LOCAL_FAKEROOT_INSTALL_INCLUDE_DIR})


##########################################################################
#                          Tesseract External Static Build               #
##########################################################################

FetchContent_Populate(tesseractPopulate
        GIT_REPOSITORY https://github.com/tesseract-ocr/tesseract.git
        GIT_TAG        5.1.0
        SOURCE_DIR "${TESSERACT_SRC_DIR}"
        TEST_COMMAND ""
)

set(TESSERACT_BUILD_ARTIFACTS_DIR "${TESSERACT_SRC_DIR}/build")
file(MAKE_DIRECTORY ${TESSERACT_BUILD_ARTIFACTS_DIR})

if(NOT TESSERACT_LIB_BUILT)
    message(STATUS "Tesseract library not built. initiating build")
    execute_process(COMMAND ${CMAKE_COMMAND} ".."  "-DCMAKE_INSTALL_PREFIX='${LOCAL_FAKEROOT_INSTALL_PREFIX}'" "-DLeptonica_DIR='${leptonica_DIR}'" "-DLeptonica_FOUND=true"
            WORKING_DIRECTORY ${TESSERACT_BUILD_ARTIFACTS_DIR}
    )
    execute_process(COMMAND ${CMAKE_COMMAND}  "--build" "." "--parallel" "${N_PROC}"
            WORKING_DIRECTORY ${TESSERACT_BUILD_ARTIFACTS_DIR}
    )

    execute_process(COMMAND ${CMAKE_COMMAND} "--install" "."
            WORKING_DIRECTORY ${TESSERACT_BUILD_ARTIFACTS_DIR}
            RESULT_VARIABLE TESSERACT_BUILD_RESULT
    )
    if(${TESSERACT_BUILD_RESULT} EQUAL 0)
        message(STATUS "Built tessaract successfully")
        set(TESSERACT_LIB_BUILT ON CACHE BOOL "" FORCE)
    endif ()
else ()
    message(STATUS "Tesseract already built. skipping")
endif ()


set(tesseract_INCLUDE_DIRS  "${LOCAL_FAKEROOT_INSTALL_INCLUDE_DIR}/tesseract")
include_directories(${tesseract_INCLUDE_DIRS})

add_library(leptonica STATIC IMPORTED)
set_target_properties(leptonica PROPERTIES
        IMPORTED_LOCATION "${LOCAL_FAKEROOT_INSTALL_LIBRARY_DIR}/libleptonica.a"
        INTERFACE_INCLUDE_DIRECTORIES "${leptonica_INCLUDE_DIRS}"
)

add_library(tesseract STATIC IMPORTED)
set_target_properties(tesseract PROPERTIES
        IMPORTED_LOCATION "${LOCAL_FAKEROOT_INSTALL_LIBRARY_DIR}/libtesseract.a"
        INTERFACE_INCLUDE_DIRECTORIES "${tesseract_INCLUDE_DIRS}"
        PUBLIC_INCLUDE_DIRECTORIES "${leptonica_INCLUDE_DIRS}"
)
##########################################################################
#                  Link Tesseract to to static leptonica  lib            #
##########################################################################
target_link_libraries(tesseract INTERFACE leptonica)

##########################################################################
#                   Add TessData Leaning files for TESSDATA_PREFIX      #
#                    This will only be achieved On Build                #
##########################################################################

ExternalProject_Add(tesseract-traineddata
        GIT_REPOSITORY https://github.com/tesseract-ocr/tessdata_fast.git
        GIT_TAG        4.1.0
        SOURCE_DIR "${TESSDATA_DIR}"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        UPDATE_COMMAND ""
        TEST_COMMAND ""
        INSTALL_COMMAND ""
)


##########################################################################
#                  Provide Targets for use by CMake File                 #
#                  that is going to import this file.                    #
##########################################################################
set(Tesseract_STATICLIB_TARGET tesseract)
set(Leptonica_STATICLIB_TARGET leptonica)
