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
#include "ns3/packet-sink.h"

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
    uint64_t txPacketsum = 0,rxPacketsum = 0, DropPacketsum = 0, LostPacketsum = 0;
    double Delaysum = 0, avgDelay=0, throughput=0, probSucc=0, probLoss=0; 
	double radius=100, appInterval=60, simulationTime=60;

  	CommandLine cmd;
  	cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  	cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
 	cmd.AddValue ("radius", "The radius of the area to simulate", radius);
 	cmd.AddValue ("appInterval", "The period in seconds to be used by periodically transmitting applications", appInterval);
  	cmd.AddValue ("simulationTime", "The time for which to simulate", simulationTime); 
  	cmd.AddValue ("file", "files containing result data", fileMetric);
 	cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  	cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  	cmd.Parse (argc,argv);

	nDevices = nCsma+nWifi;
	ofstream fileData;

  	if (verbose){
		LogComponentEnable ("simNetExemple", LOG_LEVEL_ALL);
		//LogComponentEnable ("BulkSendApplication", LOG_LEVEL_ALL);	
   		//LogComponentEnable ("PacketSink", LOG_LEVEL_ALL);	
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

  	// Mobility
  	MobilityHelper mobility;
  	mobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator", "rho", DoubleValue (radius),
                                 "X", DoubleValue (0.0), "Y", DoubleValue (0.0));
  	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  	mobility.Install (wifiStaNodes);
  	// Make it so that nodes are at a certain height > 0
  	for (NodeContainer::Iterator j = wifiStaNodes.Begin (); j != wifiStaNodes.End (); ++j){
      	Ptr<MobilityModel> mobility = (*j)->GetObject<MobilityModel> ();
      	Vector position = mobility->GetPosition ();
 		position.z = 1.2;
      	mobility->SetPosition (position);
	}
   
  	Ptr<ListPositionAllocator> allocator = CreateObject<ListPositionAllocator> ();
  	// Make it so that nodes are at a certain height > 0
  	allocator->Add (Vector (0.0, 0.0, 0.0));
  	mobility.SetPositionAllocator (allocator);
  	mobility.Install (wifiApNode);

  	// Make it so that nodes are at a certain height > 0
  	for (NodeContainer::Iterator j = wifiApNode.Begin ();
    	j != wifiApNode.End (); ++j){
      	Ptr<MobilityModel> mobility = (*j)->GetObject<MobilityModel> ();
      	Vector position = mobility->GetPosition ();
      	position.z = 15;
      	mobility->SetPosition (position);
	}


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

  	//////////////////////////////
  	// UDP Protocol Application //
  	////////////////////////////// 
   	UdpEchoServerHelper echoServer (9);

  	ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma));
  	serverApps.Start (Seconds (0));
  	serverApps.Stop (appStopTime);

  	UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma), 9);
  	//echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  	//echoClient.SetAttribute ("Interval", TimeValue (Seconds (4.0)));
  	//echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  	ApplicationContainer clientWifiApps = echoClient.Install (wifiStaNodes.Get(0));
  	for(uint32_t j=1; j < nWifi; j++){
		clientWifiApps.Add(echoClient.Install(wifiStaNodes.Get(j)));
	}
	clientWifiApps.Start (Seconds (1.0));
  	clientWifiApps.Stop (appStopTime);


/*  	ApplicationContainer clientCsmaApps = echoClient.Install (csmaNodes.Get(1));
	for(uint32_t j=2; j < nCsma; j++){
		clientCsmaApps.Add(echoClient.Install(csmaNodes.Get(j)));
	}
	clientCsmaApps.Start (Seconds (2.0));
  	clientCsmaApps.Stop (appStopTime);
*/

  	BulkSendHelper source ("ns3::TcpSocketFactory",
                         InetSocketAddress (csmaInterfaces.GetAddress (nCsma), 10));
  	// Set the amount of data to send in bytes.  Zero is unlimited.
  	source.SetAttribute ("MaxBytes", UintegerValue (512));
  	ApplicationContainer sourceApps = source.Install (csmaNodes.Get (1));
	for(int j=2; j<(int)nCsma; j++)
		sourceApps.Add(source.Install(csmaNodes.Get(j)));
  	sourceApps.Start (Seconds (2.0));
  	sourceApps.Stop (Seconds (10.0));

	//
	// Create a PacketSinkApplication and install it on node 1
	//
  	PacketSinkHelper sink ("ns3::TcpSocketFactory",
                         InetSocketAddress (Ipv4Address::GetAny (), 10));
  	ApplicationContainer sinkApps = sink.Install (csmaNodes.Get (nCsma));
  	sinkApps.Start (Seconds (1.0));
  	sinkApps.Stop (Seconds (10.0));


  	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    // install FlowMonitor to collect simulation statistics
    FlowMonitorHelper flowHelper;
    Ptr<FlowMonitor> flowMonitor = flowHelper.InstallAll();

  	Simulator::Stop (appStopTime + Seconds(2));

  	if (tracing == true){
   	   pointToPoint.EnablePcapAll ("simNet");
   	   phy.EnablePcap ("simNet", apDevices.Get (0));
  	   csma.EnablePcap ("simNet", csmaDevices.Get (0), false);
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

	throughput = (rxPacketsum*1024*8)/(simulationTime*1000);
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
    NS_LOG_INFO("  Packets Success Ratio: " << probSucc << "%");
    NS_LOG_INFO("  Packets Lost Ratio: " << probLoss << "%");	
	NS_LOG_INFO("  Throughput (Kbps): " << throughput);
  	
	fileData.open (fileMetric, ios::out | ios::app);
  	fileData << nDevices << ", " << txPacketsum << ", " << rxPacketsum << ", " << throughput << ", " << probSucc << ", " <<  probLoss  << ", " << avgDelay << "\n";
  	fileData.close();  

	return(0);
}
