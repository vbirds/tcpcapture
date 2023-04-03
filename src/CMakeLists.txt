
find_package(Threads REQUIRED)

add_executable(main main.cpp dynamiclib_mgr.cpp plugin.cpp )
target_link_libraries(main -lPacket++  -lPcap++ -lCommon++ -lpcap Threads::Threads -ldl)