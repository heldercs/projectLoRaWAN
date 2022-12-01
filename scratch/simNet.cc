/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/flow-monitor-module.h"

// Default Network Topology
//
//   Wifi 10.1.3.0
//                 AP
//  *    *    *    *
//  |    |    |    |    10.1.1.0
// n5   n6   n7   n0 -------------- n1   n2   n3   n4
//                   point-to-point  |    |    |    |
//                                   ================
//                                     LAN 10.1.2.0

using namespace ns3;
using namespace std;


NS_LOG_COMPONENT_DEFINE ("simNetExemple");

int main (int argc, char *argv[]){
	string fileMetric="./scratch/result-sim.dat";
	bool verbose = false;
  	uint32_t nCsma = 4, nWifi = 4, nDevices=0;
  	bool tracing = false;
    uint64_t txPacketsum = 0;
    uint64_t rxPacketsum = 0;
    uint64_t DropPacketsum = 0;
    uint64_t LostPacketsum = 0;
    double Delaysum = 0, avgDelay=0, throughput=0, probSucc=0, probLoss=0; 
	double simulationTime=60;


  	CommandLine cmd;
  	cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  	cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  	cmd.AddValue ("file", "files containing result data", fileMetric);
  	cmd.AddValue ("simulationTime", "The time for which to simulate", simulationTime); 
 	cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  	cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  	cmd.Parse (argc,argv);

	nDevices = nCsma+nWifi;
	ofstream fileData;

  	// The underlying restriction of 18 is due to the grid position
  	// allocator's configuration; the grid layout will exceed the
  	// bounding box if more than 18 nodes are provided.
  	//if (nWifi > 18){
    //	cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << endl;
    //  	return 1;
    //}

  	if (verbose){
   	   LogComponentEnable ("simNetExemple", LOG_LEVEL_ALL);
   	   //LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
   	   //LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

 	NodeContainer p2pNodes;
 	p2pNodes.Create (2);

 	PointToPointHelper pointToPoint;
  	pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("100Gbps"));
  	pointToPoint.SetChannelAttribute ("Delay", TimeValue(NanoSeconds(100)));

 	NetDeviceContainer p2pDevices;
 	p2pDevices = pointToPoint.Install (p2pNodes);

 	NodeContainer csmaNodes;
  	csmaNodes.Add (p2pNodes.Get (1));
  	csmaNodes.Create (nCsma);

  	CsmaHelper csma;
  	csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  	csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

 	NetDeviceContainer csmaDevices;
  	csmaDevices = csma.Install (csmaNodes);

  	NodeContainer wifiStaNodes;
  	wifiStaNodes.Create (nWifi);
  	NodeContainer wifiApNode = p2pNodes.Get (0);

  	YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  	YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  	phy.SetChannel (channel.Create ());

  	WifiHelper wifi;
  	wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  	WifiMacHelper mac;
  	Ssid ssid = Ssid ("ns-3-ssid");
  	mac.SetType ("ns3::StaWifiMac",
    	           "Ssid", SsidValue (ssid),
        	       "ActiveProbing", BooleanValue (false));

  	NetDeviceContainer staDevices;
  	staDevices = wifi.Install (phy, mac, wifiStaNodes);

  	mac.SetType ("ns3::ApWifiMac",
   	            "Ssid", SsidValue (ssid));

  	NetDeviceContainer apDevices;
  	apDevices = wifi.Install (phy, mac, wifiApNode);

  	MobilityHelper mobility;

  	mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
 	                              "MinX", DoubleValue (0.0),
   	                              "MinY", DoubleValue (0.0),
   	                              "DeltaX", DoubleValue (5.0),
   	                              "DeltaY", DoubleValue (10.0),
    	                          "GridWidth", UintegerValue (3),
        	                      "LayoutType", StringValue ("RowFirst"));

  	mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
   	                          "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  	mobility.Install (wifiStaNodes);

  	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  	mobility.Install (wifiApNode);

  	InternetStackHelper stack;
  	stack.Install (csmaNodes);
  	stack.Install (wifiApNode);
  	stack.Install (wifiStaNodes);

  	Ipv4AddressHelper address;

  	address.SetBase ("10.1.1.0", "255.255.255.0");
  	Ipv4InterfaceContainer p2pInterfaces;
  	p2pInterfaces = address.Assign (p2pDevices);

  	address.SetBase ("10.1.2.0", "255.255.255.0");
  	Ipv4InterfaceContainer csmaInterfaces;
  	csmaInterfaces = address.Assign (csmaDevices);

  	address.SetBase ("10.1.3.0", "255.255.255.0");
  	address.Assign (staDevices);
  	address.Assign (apDevices);

 	Time appStopTime = Seconds (simulationTime);

  	UdpEchoServerHelper echoServer (9);

  	ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  	serverApps.Start (Seconds (1.0));
  	serverApps.Stop (appStopTime);

  	UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
  //	echoClient.SetAttribute ("MaxPackets", UintegerValue (0));
  	echoClient.SetAttribute ("Interval", TimeValue (Seconds (120.0)));
  	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  	ApplicationContainer clientWApps = echoClient.Install (wifiStaNodes);
  	clientWApps.Start (Seconds (2.0));
  	clientWApps.Stop (appStopTime);

  	ApplicationContainer clientCApps = echoClient.Install (csmaNodes.Get(0));
	for(int j=1; j < (int)nCsma; j++){
		clientCApps.Add(echoClient.Install(csmaNodes.Get(j)));
	}
  	clientCApps.Start (Seconds (2.0));
  	clientCApps.Stop (appStopTime);

  	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    // install FlowMonitor to collect simulation statistics
    FlowMonitorHelper flowHelper;
    Ptr<FlowMonitor> flowMonitor = flowHelper.InstallAll();

  	Simulator::Stop (appStopTime);

  	if (tracing == true){
   	   pointToPoint.EnablePcapAll ("simNet");
   	   phy.EnablePcap ("simNet", apDevices.Get (0));
  	   csma.EnablePcap ("simNet", csmaDevices.Get (0), true);
  	}

  	Simulator::Run ();
  	Simulator::Destroy ();

  	/////////////////////////////
  	// Metrics results to file //
  	////////////xx///////////////
 	NS_LOG_INFO ("Computing performance metrics...");

    // get statistics of simlation from FlowMonitor
    flowMonitor->CheckForLostPackets();

    map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
	for (map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i) {
        txPacketsum += i->second.txPackets;
        rxPacketsum += i->second.rxPackets;
        LostPacketsum += i->second.lostPackets;
        DropPacketsum += i->second.packetsDropped.size();
        Delaysum += i->second.delaySum.GetSeconds();
    }

	throughput = rxPacketsum/simulationTime;
	probSucc = (rxPacketsum * 100)/txPacketsum;
    probLoss = (LostPacketsum * 100)/txPacketsum;
	avgDelay = Delaysum/txPacketsum;
    
    NS_LOG_INFO(endl << "  SIMULATION STATISTICS");
    NS_LOG_INFO("  number of Devices: " << nDevices);
    NS_LOG_INFO("  All Tx Packets: " << txPacketsum);
    NS_LOG_INFO("  All Rx Packets: " << rxPacketsum);
    NS_LOG_INFO("  All Delay: " << avgDelay);
    NS_LOG_INFO("  All Lost Packets: " << LostPacketsum);
    NS_LOG_INFO("  All Drop Packets: " << DropPacketsum);
    NS_LOG_INFO("  Packets Delivery Ratio: " << probSucc << "%");
    NS_LOG_INFO("  Packets Lost Ratio: " << probLoss << "%");	

  	fileData.open (fileMetric, ios::out | ios::app);
  	fileData << nDevices << ", " << txPacketsum << ", " << rxPacketsum << ", " << throughput << ", " << probSucc << ", " <<  probLoss  << ", " << avgDelay << "\n";
  	fileData.close();  

	return(0);
}
