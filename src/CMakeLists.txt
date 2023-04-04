
find_package(Threads REQUIRED)
if (NOT Threads_FOUND)
    message(FATAL_ERROR "Threads no found")
endif ()

add_executable(tcpcapture main.cpp dynamiclib_mgr.cpp plugin.cpp )

if(WIN32)
    target_link_libraries(tcpcapture Packet++ Pcap++ Common++ Packet wpcap Ws2_32 Iphlpapi Threads::Threads)
elseif (UNIX)
    target_link_libraries(tcpcapture -lPacket++  -lPcap++ -lCommon++ -lpcap Threads::Threads -ldl)
endif ()
