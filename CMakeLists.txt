cmake_minimum_required(VERSION 2.6)   #要求CMake的最低版本为2.8

project(muduo CXX)    #设置项目名称

if(NOT CMAKE_BUILD_TYPE)    #CMAKE_BUILD_TYPE默认为Debug
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CXX_FLAGS           #设置编译参数
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 -D_FILE_OFFSET_BITS=64
 -Wall
 -Wextra
 -Werror
 -Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 -std=c++11
 -rdynamic
 )
if(CMAKE_BUILD_BITS EQUAL 32)
  list(APPEND CXX_FLAGS "-m32")
endif()
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")  #设置编译参数

set(CMAKE_CXX_COMPILER "g++")       #编译器类型
#set(CMAKE_CXX_COMPILER "icpc")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")    
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)    #设置可行性文件目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)       #库文件目录

find_package(Boost REQUIRED)        #寻找第三方库
find_package(Protobuf)
find_package(CURL)
find_path(CARES_INCLUDE_DIR ares.h)
find_library(CARES_LIBRARY NAMES cares)
find_path(MHD_INCLUDE_DIR microhttpd.h)
find_library(MHD_LIBRARY NAMES microhttpd)
find_library(BOOSTTEST_LIBRARY NAMES boost_unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "CXX_FLAGS = " ${CMAKE_CXX_FLAGS} " " ${CMAKE_CXX_FLAGS_${BUILD_TYPE}})

add_subdirectory(muduo/base)
#add_subdirectory(muduo/net)

#if(NOT CMAKE_BUILD_NO_EXAMPLES)
#  add_subdirectory(examples)
#else()
#  if(CARES_INCLUDE_DIR AND CARES_LIBRARY)
#    add_subdirectory(examples/cdns)
#  endif()
#endif()

#add_subdirectory(tests)
