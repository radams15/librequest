cmake_minimum_required(VERSION 2.8)
project(Request C CXX)

option(USE_LIBCURL "Uses libcurl library (must use for linux)" OFF)
option(USE_LOCAL_CERT "Uses local certificate bundle called 'curl-ca-bundle.crt' (only for libcurl)" OFF)

if(UNIX AND NOT APPLE)
    set(USE_LIBCURL ON)
endif()

set(LIBS src/main/res.c src/main/header.c src/main/request_generic.c)

set(LIBREQ_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

if(USE_LIBCURL)
    message("Using libcurl")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CURL REQUIRED libcurl)

    set(LIBS ${LIBS} src/main/backends/curl/request.c)
else()
    if(APPLE)
        message("Using NSUrlRequest")
        set(LIBS ${LIBS} src/main/backends/darwin/request.m)
        find_library(FOUNDATION foundation REQUIRED)
    endif()

    if(WIN32)
        message("Using wininet")
        set(LIBS ${LIBS} src/main/backends/win32/request.cpp)
        set(WIN32_LIBS -lwininet)
    endif()
endif()

add_library(request STATIC ${LIBS})

if(USE_LIBCURL AND USE_LOCAL_CERT)
    message("Using local certificate bundle")
    target_compile_definitions(request PRIVATE LOCAL_CERT)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_definitions(request PUBLIC NO_EXPORT)

message("Libs: ${LIBS}")

add_subdirectory(lib/b64.c)

target_include_directories(request PRIVATE include/ src/main/ ${CURL_INCLUDE_DIRS} ${B64_INCLUDES})

target_link_libraries(request PRIVATE b64c ${FOUNDATION} ${CURL_LIBRARIES} ${WIN32_LIBS})