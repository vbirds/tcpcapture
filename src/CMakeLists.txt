
find_package(Threads REQUIRED)
if (NOT Threads_FOUND)
    message(FATAL_ERROR "Threads no found")
endif ()

add_executable(main main.cpp dynamiclib_mgr.cpp plugin.cpp )
target_link_libraries(main -lPacket++  -lPcap++ -lCommon++ -lpcap Threads::Threads -ldl)