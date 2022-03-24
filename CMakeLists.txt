cmake_minimum_required(VERSION 2.8)
project(Request C)

option(BUILD_TEST "Builds a test app" ON)
option(USE_LIBCURL "Uses libcurl library (must use for linux)" OFF)

if(UNIX AND NOT APPLE)
    set(USE_LIBCURL ON)
endif()

set(LIBS res.c header.c)

set(LIBREQ_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)

if(USE_LIBCURL)
    message("Using libcurl")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CURL REQUIRED libcurl)

    set(LIBS ${LIBS} curl/request.c)
else()
    if(APPLE)
        message("Using NSUrlRequest")
        set(LIBS ${LIBS} darwin/request.m)
        find_library(FOUNDATION foundation REQUIRED)
    endif()

    if(WIN32)
        message("Using wininet")
        set(LIBS ${LIBS} win32/request.cpp curl/request.c)
    endif()
endif()

add_library(request SHARED ${LIBS})

add_subdirectory(lib/b64)

target_include_directories(request PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CURL_INCLUDE_DIRS} lib/b64/)

target_link_libraries(request PRIVATE b64c ${FOUNDATION} ${CURL_LIBRARIES})

if(BUILD_TEST)
    add_executable(reqtest test.c)
    target_include_directories(reqtest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(reqtest request)
endif()