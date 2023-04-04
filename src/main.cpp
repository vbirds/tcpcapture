
#include "pcapplusplus/TcpReassembly.h"
#include "pcapplusplus/PcapLiveDeviceList.h"
#include "pcapplusplus/SystemUtils.h"
#include "pcapplusplus/PcapPlusPlusVersion.h"
#include "cmdlineparse.hh"
#include "plugin.h"

#include <iostream>
#include <algorithm>

Plugin g_plugin;

#define EXIT_WITH_ERROR(reason) do { \
	std::cout << std::endl << "ERROR: " << reason << std::endl << std::endl; \
	exit(1); \
	} while(0)

#if defined(_WIN32)
#define SEPARATOR '\\'
#else
#define SEPARATOR '/'
#endif

void conv_connection_data(connection_data & key, const pcpp::ConnectionData& data)
{
    key.src_ip = data.srcIP.getIPv4().toInt();
    key.dst_ip = data.dstIP.getIPv4().toInt();
    key.src_port = data.srcPort;
    key.dst_port = data.dstPort;
    key.flowKey = data.flowKey;
    key.startTime = data.startTime;
    key.endTime = data.endTime;
}

static void on_application_interrupted(void* cookie)
{
    bool* shouldStop = (bool*)cookie;
    *shouldStop = true;
}

static void on_packet_arrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice* dev, void* tcpReassemblyCookie)
{
    // get a pointer to the TCP reassembly instance and feed the packet arrived to it
    pcpp::TcpReassembly* tcpReassembly = (pcpp::TcpReassembly*)tcpReassemblyCookie;
    tcpReassembly->reassemblePacket(packet);
}

void tcp_reassembly_loop(pcpp::PcapLiveDevice* dev, pcpp::TcpReassembly& tcpReassembly, const std::string& bpfFilter = "")
{
    // try to open device
    if (!dev->open())
        EXIT_WITH_ERROR("Cannot open interface");

    // set BPF filter if set by the user
    if (!bpfFilter.empty())
    {
        if (!dev->setFilter(bpfFilter))
            EXIT_WITH_ERROR("Cannot set BPF filter to interface");
    }

    std::cout << "Starting packet capture on '" << dev->getIPv4Address() << "'..." << "bpfFilter:" << bpfFilter << std::endl;

    // start capturing packets. Each packet arrived will be handled by onPacketArrives method
    dev->startCapture(on_packet_arrives, &tcpReassembly);

    // register the on app close event to print summary stats on app termination
    bool shouldStop = false;
    pcpp::ApplicationEventHandler::getInstance().onApplicationInterrupted(on_application_interrupted, &shouldStop);

    // run in an endless loop until the user presses ctrl+c
    while(!shouldStop)
        pcpp::multiPlatformSleep(1);

    // stop capturing and close the live device
    dev->stopCapture();
    dev->close();

    // close all connections which are still opened
    tcpReassembly.closeAllConnections();

    std::cout << "Done! processed " << tcpReassembly.getConnectionInformation().size() << " connections" << std::endl;
}

static void tcp_reassembly_connection_end_callback(const pcpp::ConnectionData& connectionData, pcpp::TcpReassembly::ConnectionEndReason reason, void* userCookie)
{
    connection_data key;
    conv_connection_data(key, connectionData);
    g_plugin.plugin_connection_close(&key);
}

static void tcp_reassembly_connection_start_callback(const pcpp::ConnectionData& connectionData, void* userCookie)
{
    connection_data key;
    conv_connection_data(key, connectionData);
    g_plugin.plugin_connection_start(&key);
}

static void tcp_reassembly_msg_ready_callback(int8_t sideIndex, const pcpp::TcpStreamData& tcpData, void* userCookie)
{
    connection_data key;
    conv_connection_data(key, tcpData.getConnectionData());
    if (sideIndex == 0)
    {
       // client -> server;
        g_plugin.plugin_connection_recv(&key, (char*)tcpData.getData(), tcpData.getDataLength());
    }
    else
    {
        // server -> client;
        g_plugin.plugin_connection_send(&key, (char*)tcpData.getData(), tcpData.getDataLength());
    }
}

int main(int argc, char* argv[])
{
    pcpp::AppName::init(argc, argv);
    cmdlineparse::parser cmd;

    std::string device;
    std::string ip;
    std::string port;
    std::string plugin;
    bool show = false;
    cmd.add_default();

    cmd.add_string('d', "device", &device, "", "device name, default use first eth device");
    cmd.add_string('i', "ip", &ip, "", "<ip address>");
    cmd.add_string('p', "port", &port, "", "<port>");
    cmd.add_string('P', "plugin", &plugin, "", "plugin shared library name");
    cmd.add_flag('s', "show-device", &show, "", "show system device list");

    // Parse
    if (!cmd.parse (argc, argv))
    {
        std::cout << cmd.build_usage() << std::endl;
        return 1;
    }

    if (show)
    {
        const std::vector<pcpp::PcapLiveDevice*> & devVec= pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
        if (devVec.empty())
        {
            printf("No devices found\n");
            return -1;
        }
        printf("device lists:\n");
        for (auto dev : devVec)
        {
            printf("%s ip:%s\n", dev->getName().c_str(), dev->getIPv4Address().toString().c_str());
        }
        return 0; }

    if (ip.empty() && port.empty())
    {
        std::cout << cmd.build_help() << std::endl;
        return 1;
    }

    if (device.empty())
    {
        const std::vector<pcpp::PcapLiveDevice*> & devVec= pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
        if (devVec.empty())
        {
            printf("No devices found\n");
            return -1;
        }
        device = devVec[0]->getName();
    }

    pcpp::PcapLiveDevice* dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIpOrName(device);
    if (dev == nullptr) {
        EXIT_WITH_ERROR("Couldn't find interface by provided IP address or name");
        return -1;
    }

    if (!plugin.empty()) {
        int rc = g_plugin.load(plugin.c_str());
        if (rc != 0)
        {
            printf("plugin load failed rc:%d\n", rc);
            return -1;
        }

        // init
        rc = g_plugin.plugin_init();
        if (rc != 0)
        {
            printf("plugin_init failed rc:%d\n", rc);
            return -1;
        }

    }

    std::stringstream ssFilter;
    if (!ip.empty())
    {
        ssFilter << "host " << ip;
    }

    if (!port.empty())
    {
        if (!ip.empty())
        {
            ssFilter << " and ";
        }
        ssFilter << " port " << port;
    }

    // create the TCP reassembly instance
    pcpp::TcpReassembly tcpReassembly(tcp_reassembly_msg_ready_callback, nullptr, tcp_reassembly_connection_start_callback, tcp_reassembly_connection_end_callback);

    tcp_reassembly_loop(dev, tcpReassembly, ssFilter.str());
    if (!plugin.empty()) {
        g_plugin.plugin_close();
    }

    return 0;
}