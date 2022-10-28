cmake_minimum_required(VERSION 2.8)
project(Request C CXX)

option(BACKEND "Backend for web requests curl/" curl)
option(USE_LOCAL_CERT "Uses local certificate bundle called 'curl-ca-bundle.crt' (only for libcurl)" OFF)


set(LIBS src/main/res.c src/main/header.c src/main/request_generic.c lib/b64.c/buffer.c lib/b64.c/decode.c lib/b64.c/encode.c)

set(LIBREQ_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)


if("${BACKEND}" STREQUAL "curl")
    message("Using libcurl backend")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(CURL REQUIRED libcurl)

    set(LIBS ${LIBS} src/main/backends/curl/request.c)
elseif("${BACKEND}" STREQUAL "raw")
    message("Using raw backend")

    set(LIBS ${LIBS} src/main/backends/raw/request.cpp)
elseif("${BACKEND}" STREQUAL "qt")
    message("Using qt backend")

    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

    set(${QT5_LIBS} Qt5::Core Qt5::Network)

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)


    set(LIBS ${LIBS} src/main/backends/qt/request.cpp)
else()
    if(APPLE)
        message("Using NSUrlRequest backend")
        set(LIBS ${LIBS} src/main/backends/darwin/request.m)
        find_library(FOUNDATION foundation REQUIRED)
    elseif(WIN32)
        message("Using wininet backend")
        set(LIBS ${LIBS} src/main/backends/win32/request.cpp)
        set(WIN32_LIBS -lwininet)
    else()
        message("Using libcurl backend")
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(CURL REQUIRED libcurl)

        set(LIBS ${LIBS} src/main/backends/curl/request.c)
    endif()
endif()

add_library(request SHARED ${LIBS})

if(USE_LIBCURL AND USE_LOCAL_CERT)
    message("Using local certificate bundle")
    target_compile_definitions(request PRIVATE LOCAL_CERT)
endif()

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
target_compile_definitions(request PUBLIC NO_EXPORT)

message("Libs: ${LIBS}")

add_subdirectory(lib/b64.c)

target_include_directories(request PRIVATE include/ src/main/ ${CURL_INCLUDE_DIRS} ${B64_INCLUDES})

target_link_libraries(request PRIVATE ${FOUNDATION} ${CURL_LIBRARIES} ${WIN32_LIBS} ${QT5_LIBS})

if("${BACKEND}" STREQUAL "qt")
qt5_use_modules(request Core Network)
endif()

add_executable(test_req src/main/test.c)
target_link_libraries(test_req request)
target_include_directories(test_req PRIVATE include)