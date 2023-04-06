cmake_minimum_required(VERSION 2.8)
project(tcpcapture)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories(${PROJECT_SOURCE_DIR}/thirdparh/spdlog/include)

if(WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparh/winpcap/include)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparh/winpcapplusplus/include)

    link_directories(${PROJECT_SOURCE_DIR}/thirdparh/winpcap/lib64)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        # Debug 模式
        link_directories(${PROJECT_SOURCE_DIR}/thirdparh/winpcapplusplus/lib64/debug)
    else()
        link_directories(${PROJECT_SOURCE_DIR}/thirdparh/winpcapplusplus/lib64/release)
    endif()

elseif(UNIX)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparh/pcap/include)
    include_directories(${PROJECT_SOURCE_DIR}/thirdparh/pcapplusplus/include)
    link_directories(${PROJECT_SOURCE_DIR}/thirdparh/pacp/lib)
    link_directories(${PROJECT_SOURCE_DIR}/thirdparh/pcapplusplus/lib64)
endif ()

add_subdirectory(src)
add_subdirectory(plugins)
