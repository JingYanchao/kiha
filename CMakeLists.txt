cmake_minimum_required(VERSION 3.7)
project(kiha)
if(NOT MUDUO_PATH)
    if(CMAKE_BUILD_TYPE EQUAL Debug)
        set(MUDUO_PATH "/home/jyc/build/debug-install-cpp11")
    else()
        set(MUDUO_PATH "/home/jyc/build/release-install-cpp11")
    endif()
endif()
message(${MUDUO_PATH})
set(CXX_FLAGS
        -g
        # -DVALGRIND
        # -DMUDUO_STD_STRING
        -Wall
        -Wextra
        # -m32
        #-Werror
        #-Wconversion
        -Wno-unused-parameter
        # -Wold-style-cast
        -Woverloaded-virtual
        -Wpointer-arith
        -Wshadow
        -Wwrite-strings
        -march=native
        # -MMD
        -std=c++11
        -rdynamic
        )
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

find_path(Muduo_INCLUDE_DIR muduo "${MUDUO_PATH}/include")
find_path(Muduo_LIBRARY_DIR libmuduo_net.a "${MUDUO_PATH}/lib")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${Muduo_LIBRARY_DIR})
message(STATUS ${Muduo_INCLUDE_DIR})
message(STATUS ${Muduo_LIBRARY_DIR})

include_directories(${Muduo_INCLUDE_DIR})
find_library(muduo_base muduo_base)
find_library(muduo_net muduo_net)
message(STATUS ${muduo_base})
message(STATUS ${muduo_net})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

add_executable(HttpServer
        Server.cc Server.h
        http/HttpRequest.h
        copyable.h
        http/HttpResponse.h
        http/Http.h http/Http.cc
        noncopyable.h
        http/HttpParser.h http/HttpParser.cc testServer.cc http/HttpResponse.cc)
target_link_libraries(HttpServer ${muduo_base} ${muduo_net} pthread rt pcap tcmalloc)