cmake_minimum_required(VERSION 2.8)
project(Request C)

option(BUILD_TEST "Builds a test app" ON)

set(LIBS res.c header.c)

set(LIBREQ_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/ PARENT_SCOPE)

if(APPLE)
    set(LIBS ${LIBS} darwin/request.m)
    find_library(FOUNDATION foundation REQUIRED)
endif()

if(WIN32)
    set(LIBS ${LIBS} win32/request.cpp)
endif()

add_library(request SHARED ${LIBS})

add_subdirectory(lib/b64)

target_include_directories(request PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} lib/b64/)

target_link_libraries(request PRIVATE b64c ${FOUNDATION})

if(BUILD_TEST)
    add_executable(reqtest test.c)
    target_include_directories(reqtest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(reqtest request)
endif()