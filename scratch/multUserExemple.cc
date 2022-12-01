/*
 * =====================================================================================
 *
 *       Filename:  simNetMultUser.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18/01/2022 17:35:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  Federal University of Ceara
 *
 * =====================================================================================
 */

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-grid.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("UdpMultiClient");

int main(int argc, char *argv[]) {


   	// Set up logging
    LogComponentEnable ("UdpMultiClient", LOG_LEVEL_ALL);
 
    // create grid structure of network
    // not necessary. Could just create topology manually
    PointToPointHelper p2pLink;
    PointToPointGridHelper grid (2, 2, p2pLink);
    InternetStackHelper stack;
    grid.InstallStack(stack);

    // assign IP addresses to NetDevices
    grid.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"),
                              Ipv4AddressHelper ("10.2.1.0", "255.255.255.0"));
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // configure and install server app
    int serverPort = 8080;
    UdpEchoServerHelper serverApp (serverPort);
    serverApp.Install(grid.GetNode(0,0));
    Address serverAddress = InetSocketAddress(grid.GetIpv4Address(0,0), serverPort);

    // configure and install client apps
    UdpEchoClientHelper clientApp (serverAddress);
    clientApp.Install(grid.GetNode(0,1));
    clientApp.Install(grid.GetNode(1,0));
    clientApp.Install(grid.GetNode(1,1));

    // install FlowMonitor to collect simulation statistics
    FlowMonitorHelper flowHelper;
    Ptr<FlowMonitor> flowMonitor = flowHelper.InstallAll();

    // configure and run simulation
    Simulator::Stop(Seconds(20));
    NS_LOG_UNCOND("Starting simulation.");
    Simulator::Run();
    Simulator::Destroy();
    NS_LOG_UNCOND("Simulation completed.");

    // simulation complete
    // get statistics of simlation from FlowMonitor
    flowMonitor->CheckForLostPackets();
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
    uint64_t txPacketsum = 0;
    uint64_t rxPacketsum = 0;
    uint64_t DropPacketsum = 0;
    uint64_t LostPacketsum = 0;
    double Delaysum = 0;

	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i) {
        txPacketsum += i->second.txPackets;
        rxPacketsum += i->second.rxPackets;
        LostPacketsum += i->second.lostPackets;
        DropPacketsum += i->second.packetsDropped.size();
        Delaysum += i->second.delaySum.GetSeconds();
    }

    NS_LOG_UNCOND(std::endl << "  SIMULATION STATISTICS");
    NS_LOG_UNCOND("  All Tx Packets: " << txPacketsum);
    NS_LOG_UNCOND("  All Rx Packets: " << rxPacketsum);
    NS_LOG_UNCOND("  All Delay: " << Delaysum / txPacketsum);
    NS_LOG_UNCOND("  All Lost Packets: " << LostPacketsum);
    NS_LOG_UNCOND("  All Drop Packets: " << DropPacketsum);
    NS_LOG_UNCOND("  Packets Delivery Ratio: " << ((rxPacketsum * 100) / txPacketsum) << "%");
    NS_LOG_UNCOND("  Packets Lost Ratio: " << ((LostPacketsum * 100) / txPacketsum) << "%");
    // flowMonitor->SerializeToXmlFile("test.xml", true, true);

    return 0;
}
