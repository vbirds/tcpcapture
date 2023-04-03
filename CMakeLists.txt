cmake_minimum_required(VERSION 2.8)
project(tcpcapture)

set(CMAKE_CXX_STANDARD 11)


include_directories(${PROJECT_SOURCE_DIR}/include)

#include_directories(${PROJECT_SOURCE_DIR}/thirdparh/tins/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparh/spdlog/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparh/pcap/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparh/dsf/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparh/pcapplusplus/include)

#link_directories(${PROJECT_SOURCE_DIR}/thirdparh/tins/lib)
link_directories(${PROJECT_SOURCE_DIR}/thirdparh/pacp/lib)
link_directories(${PROJECT_SOURCE_DIR}/thirdparh/dsf/lib)
link_directories(${PROJECT_SOURCE_DIR}/thirdparh/pcapplusplus/lib64)


add_subdirectory(src)
