/*
 * =====================================================================================
 *
 *       Filename:  lorawan-network-mClass-sim.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/06/2020 22:42:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder (FHC), helderhdw@gmail.com
 *   Organization:  Federal University of Ceara
 *
 * =====================================================================================
 */

#include "ns3/end-device-lora-phy.h"
#include "ns3/gateway-lora-phy.h"
#include "ns3/class-a-end-device-lorawan-mac.h"
#include "ns3/gateway-lorawan-mac.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/lora-helper.h"
#include "ns3/node-container.h"
#include "ns3/mobility-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/double.h"
#include "ns3/random-variable-stream.h"
#include "ns3/periodic-sender-helper.h"
#include "ns3/random-sender-helper.h"
#include "ns3/command-line.h"
#include "ns3/network-server-helper.h"
#include "ns3/correlated-shadowing-propagation-loss-model.h"
#include "ns3/building-penetration-loss.h"
#include "ns3/building-allocator.h"
#include "ns3/buildings-helper.h"
#include "ns3/forwarder-helper.h"
#include <algorithm>
#include <ctime>

using namespace ns3;
using namespace lorawan;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("LorawanNetworkSimulator");

#define MAXRTX 4

// Network settings
int nDevices = 200;
int nRegulars = 150;
int nAlarms = 10;
int nGateways = 1;
double radius = 7500;
double gatewayRadius = 0;
double simulationTime = 600;

// Channel model
bool realisticChannelModel = false;

int appPeriodSeconds = 600;

// Output control
bool printBuildings = false;
bool printEDs = true;

enum SF { SF7=7, SF8, SF9, SF10, SF11, SF12 };

enum nodeType { ALARM, REGULAR };

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  printEndDevices
 *  Description:  
 * =====================================================================================
 */
void PrintEndDevices (NodeContainer endDevices, NodeContainer gateways, std::string filename1, std::string filename2, std::string filename3){
  	const char * c = filename1.c_str ();
  	std::ofstream spreadingFactorFile;
  	spreadingFactorFile.open (c);
	// print for regular event
  	for (int j = 0; j < nRegulars; ++j){
    	Ptr<Node> object = endDevices.Get(j);
      	Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
      	NS_ASSERT (position != 0);
      	Ptr<NetDevice> netDevice = object->GetDevice (0);
      	Ptr<LoraNetDevice> loraNetDevice = netDevice->GetObject<LoraNetDevice> ();
      	NS_ASSERT (loraNetDevice != 0);
      	Ptr<EndDeviceLorawanMac> mac = loraNetDevice->GetMac ()->GetObject<EndDeviceLorawanMac> ();
      	int sf = int(mac->GetSfFromDataRate(mac->GetDataRate ()));
      	Vector pos = position->GetPosition ();
      	spreadingFactorFile << pos.x << " " << pos.y << " " << sf << endl;
  	}
	spreadingFactorFile.close ();
  	
  	c = filename2.c_str ();
  	spreadingFactorFile.open (c);
	// print for alarm event
  	for (int j = nRegulars; j < nDevices; ++j){
    	Ptr<Node> object = endDevices.Get(j);
      	Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
      	NS_ASSERT (position != 0);
      	Ptr<NetDevice> netDevice = object->GetDevice (0);
      	Ptr<LoraNetDevice> loraNetDevice = netDevice->GetObject<LoraNetDevice> ();
      	NS_ASSERT (loraNetDevice != 0);
      	Ptr<EndDeviceLorawanMac> mac = loraNetDevice->GetMac ()->GetObject<EndDeviceLorawanMac> ();
      	int sf = int(mac->GetSfFromDataRate(mac->GetDataRate ()));
      	Vector pos = position->GetPosition ();
      	spreadingFactorFile << pos.x << " " << pos.y << " " << sf << endl;
  	}
  	spreadingFactorFile.close ();
  	
	c = filename3.c_str ();
  	spreadingFactorFile.open (c);	
  	// Also print the gateways
  	for (NodeContainer::Iterator j = gateways.Begin (); j != gateways.End (); ++j){
    	Ptr<Node> object = *j;
      	Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
      	Vector pos = position->GetPosition ();
      	spreadingFactorFile << pos.x << " " << pos.y << " GW" << endl;
  	}
  	spreadingFactorFile.close ();
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  buildingHandler
 *  Description:  
 * =====================================================================================
 */
void buildingHandler ( NodeContainer endDevices, NodeContainer gateways ){

	double xLength = 230;
  	double deltaX = 80;
  	double yLength = 164;
  	double deltaY = 57;
 	int gridWidth = 2 * radius / (xLength + deltaX);
  	int gridHeight = 2 * radius / (yLength + deltaY);

  	if (realisticChannelModel == false){
    	gridWidth = 0;
    	gridHeight = 0;
    }
  
	Ptr<GridBuildingAllocator> gridBuildingAllocator;
  	gridBuildingAllocator = CreateObject<GridBuildingAllocator> ();
  	gridBuildingAllocator->SetAttribute ("GridWidth", UintegerValue (gridWidth));
  	gridBuildingAllocator->SetAttribute ("LengthX", DoubleValue (xLength));
  	gridBuildingAllocator->SetAttribute ("LengthY", DoubleValue (yLength));
  	gridBuildingAllocator->SetAttribute ("DeltaX", DoubleValue (deltaX));
  	gridBuildingAllocator->SetAttribute ("DeltaY", DoubleValue (deltaY));
  	gridBuildingAllocator->SetAttribute ("Height", DoubleValue (6));
  	gridBuildingAllocator->SetBuildingAttribute ("NRoomsX", UintegerValue (2));
  	gridBuildingAllocator->SetBuildingAttribute ("NRoomsY", UintegerValue (4));
  	gridBuildingAllocator->SetBuildingAttribute ("NFloors", UintegerValue (2));
  	gridBuildingAllocator->SetAttribute (
      "MinX", DoubleValue (-gridWidth * (xLength + deltaX) / 2 + deltaX / 2));
  	gridBuildingAllocator->SetAttribute (
      "MinY", DoubleValue (-gridHeight * (yLength + deltaY) / 2 + deltaY / 2));
  	BuildingContainer bContainer = gridBuildingAllocator->Create (gridWidth * gridHeight);

  	BuildingsHelper::Install (endDevices);
  	BuildingsHelper::Install (gateways);
    BuildingsHelper::MakeMobilityModelConsistent ();

  	// Print the buildings
  	if (printBuildings){
    	std::ofstream myfile;
    	myfile.open ("buildings.txt");
      	std::vector<Ptr<Building>>::const_iterator it;
      	int j = 1;
      	for (it = bContainer.Begin (); it != bContainer.End (); ++it, ++j){
			Box boundaries = (*it)->GetBoundaries ();
        	myfile << "set object " << j << " rect from " << boundaries.xMin << "," << boundaries.yMin
                 << " to " << boundaries.xMax << "," << boundaries.yMax << std::endl;
      	}
      	myfile.close ();
    }

}/* -----  end of function buildingHandler  ----- */


int main (int argc, char *argv[]){

	string fileSF7Metric="./scratch/result-STAs.dat";
	string fileSF8Metric="./scratch/result-STAs.dat";
	string fileSF9Metric="./scratch/result-STAs.dat";
 	string fileData="./scratch/mac-STAs-GW-1.txt";
	string endDevRegFile="./TestResult/test";
	string endDevAlmFile="./TestResult/test";
	string gwFile="./TestResult/test";
	bool flagRtx=0;
  	uint32_t nSeed=1;
	int trial=1; //, numRTX=0;
	vector<int> sfQuant(6,0);
	double packLoss=0, sent=0, received=0, avgDelay=0;
	double angle=0, sAngle=M_PI;
	double throughput=0, probSucc=0, probLoss=0;

	CommandLine cmd;
  	cmd.AddValue ("nSeed", "Number of seed to position", nSeed);
  	cmd.AddValue ("nDevices", "Number of end devices to include in the simulation", nDevices);
  	cmd.AddValue ("nGateways", "Number of gateway rings to include", nGateways);
  	cmd.AddValue ("radius", "The radius of the area to simulate", radius);
  	cmd.AddValue ("gatewayRadius", "The distance between gateways", gatewayRadius);
  	cmd.AddValue ("simulationTime", "The time for which to simulate", simulationTime);
  	cmd.AddValue ("appPeriod", "The period in seconds to be used by periodically transmitting applications", appPeriodSeconds);
  	cmd.AddValue ("file1", "files containing result data", fileSF7Metric);
   	cmd.AddValue ("file2", "files containing result data", fileSF8Metric);
  	cmd.AddValue ("file3", "files containing result data", fileSF9Metric);
  	cmd.AddValue ("file4", "files containing result information", fileData);
  	cmd.AddValue ("printEDs", "Whether or not to print various informations", printEDs);
  	cmd.AddValue ("trial", "set trial parameter", trial);
  	cmd.Parse (argc, argv);

	
	nRegulars = 0.95*nDevices; 
	nAlarms = nDevices - nRegulars;

	NS_LOG_DEBUG("number regular event: " << nRegulars << "number alarm event: " << nAlarms );


	endDevRegFile += to_string(trial) + "/endDevicesReg" + to_string(nRegulars) + ".dat";
	endDevAlmFile += to_string(trial) + "/endDevicesAlm" + to_string(nAlarms) + ".dat";
	gwFile += to_string(trial) + "/GWs" + to_string(nGateways) + ".dat";

  	// Set up logging
  	// LogComponentEnable ("LorawanNetworkSimulator", LOG_LEVEL_ALL);
  	// LogComponentEnable("LoraPacketTracker", LOG_LEVEL_ALL);
  	// LogComponentEnable("LoraChannel", LOG_LEVEL_INFO);
  	// LogComponentEnable("LoraPhy", LOG_LEVEL_ALL);
  	// LogComponentEnable("EndDeviceLoraPhy", LOG_LEVEL_ALL);
  	// LogComponentEnable("GatewayLoraPhy", LOG_LEVEL_ALL);
  	// LogComponentEnable("LoraInterferenceHelper", LOG_LEVEL_ALL);
  	// LogComponentEnable("LorawanMac", LOG_LEVEL_ALL);
  	// LogComponentEnable("EndDeviceLorawanMac", LOG_LEVEL_ALL);
  	// LogComponentEnable("ClassAEndDeviceLorawanMac", LOG_LEVEL_ALL);
  	// LogComponentEnable("GatewayLorawanMac", LOG_LEVEL_ALL);
  	// LogComponentEnable("LogicalLoraChannelHelper", LOG_LEVEL_ALL);
  	// LogComponentEnable("LogicalLoraChannel", LOG_LEVEL_ALL);
  	// LogComponentEnable("LoraHelper", LOG_LEVEL_ALL);
  	// LogComponentEnable("LoraPhyHelper", LOG_LEVEL_ALL);
  	// LogComponentEnable("LorawanMacHelper", LOG_LEVEL_ALL);
  	// LogComponentEnable("PeriodicSenderHelper", LOG_LEVEL_ALL);
  	// LogComponentEnable("PeriodicSender", LOG_LEVEL_ALL);
   	// LogComponentEnable("RandomSenderHelper", LOG_LEVEL_ALL);
  	// LogComponentEnable("RandomSender", LOG_LEVEL_ALL);
  	// LogComponentEnable("LorawanMacHeader", LOG_LEVEL_ALL);
  	// LogComponentEnable("LoraFrameHeader", LOG_LEVEL_ALL);
  	// LogComponentEnable("NetworkScheduler", LOG_LEVEL_ALL);
  	// LogComponentEnable("NetworkServer", LOG_LEVEL_ALL);
  	// LogComponentEnable("NetworkStatus", LOG_LEVEL_ALL);
  	// LogComponentEnable("NetworkController", LOG_LEVEL_ALL);

  	/***********
   	*  Setup  *
   	***********/
	ofstream myfile;

  	RngSeedManager::SetSeed(1);
  	RngSeedManager::SetRun(nSeed);

  	// Create the time value from the period
  	Time appPeriod = Seconds (appPeriodSeconds);

  	// Mobility
  	MobilityHelper mobility;
  	mobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator", "rho", DoubleValue (radius),
                                 "X", DoubleValue (0.0), "Y", DoubleValue (0.0));
  	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  	/************************
   	*  Create the channel  *
   	************************/

  	// Create the lora channel object
  	Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel> ();
  	loss->SetPathLossExponent (3.76);
  	loss->SetReference (1, 7.7);

  	if (realisticChannelModel){
      	// Create the correlated shadowing component
      	Ptr<CorrelatedShadowingPropagationLossModel> shadowing =
          	CreateObject<CorrelatedShadowingPropagationLossModel> ();

      	// Aggregate shadowing to the logdistance loss
      	loss->SetNext (shadowing);

      	// Add the effect to the channel propagation loss
      	Ptr<BuildingPenetrationLoss> buildingLoss = CreateObject<BuildingPenetrationLoss> ();

      	shadowing->SetNext (buildingLoss);
    }

  	Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();

  	Ptr<LoraChannel> channel = CreateObject<LoraChannel> (loss, delay);

  	/************************
   	*  Create the helpers  *
   	************************/

  	// Create the LoraPhyHelper
  	LoraPhyHelper phyHelper = LoraPhyHelper ();
  	phyHelper.SetChannel (channel);

 	// Create the LorawanMacHelper
  	LorawanMacHelper macHelper = LorawanMacHelper ();

  	// Create the LoraHelper
  	LoraHelper helper = LoraHelper ();
  	helper.EnablePacketTracking (); // Output filename
  	// helper.EnableSimulationTimePrinting ();

  	//Create the NetworkServerHelper
  	NetworkServerHelper nsHelper = NetworkServerHelper ();

  	//Create the ForwarderHelper
  	ForwarderHelper forHelper = ForwarderHelper ();

  	/************************
   	*  Create End Devices  *
   	************************/

  	// Create a set of nodes
  	NodeContainer endDevices;
  	endDevices.Create (nDevices);

  	// Assign a mobility model to each node
  	mobility.Install (endDevices);
  	//int x =5000.00, y= 0;
  	// Make it so that nodes are at a certain height > 0
  	for (NodeContainer::Iterator j = endDevices.Begin (); j != endDevices.End (); ++j){
      	Ptr<MobilityModel> mobility = (*j)->GetObject<MobilityModel> ();
      	Vector position = mobility->GetPosition ();
	  	//position.x = x;
		//position.y = y;
      	position.z = 1.2;
      	mobility->SetPosition (position);
		//x +=2500;
    }


  	// Create the LoraNetDevices of the end devices
  	uint8_t nwkId = 54;
  	uint32_t nwkAddr = 1864;
  	Ptr<LoraDeviceAddressGenerator> addrGen =
    	  CreateObject<LoraDeviceAddressGenerator> (nwkId, nwkAddr);

 	// Create the LoraNetDevices of the end devices
  	macHelper.SetAddressGenerator (addrGen);
  	phyHelper.SetDeviceType (LoraPhyHelper::ED);
  	macHelper.SetDeviceType (LorawanMacHelper::ED_A);
  	helper.Install (phyHelper, macHelper, endDevices);

  	// Now end devices are connected to the channel

 	// Connect trace sources for Regulars
  	for (int j = 0; j < nRegulars; ++j){
			Ptr<Node> node = endDevices.Get(j);
			Ptr<LoraNetDevice> loraNetDevice = node->GetDevice (0)->GetObject<LoraNetDevice> ();
			Ptr<LoraPhy> phy = loraNetDevice->GetPhy ();
	}
  	
	// Connect trace sources for Alarms
  	for (int j = nRegulars; j < nDevices; ++j){
			Ptr<Node> node = endDevices.Get(j);
			Ptr<LoraNetDevice> loraNetDevice = node->GetDevice (0)->GetObject<LoraNetDevice> ();
			Ptr<LoraPhy> phy = loraNetDevice->GetPhy ();
			
			if (flagRtx){
				Ptr<EndDeviceLorawanMac> mac = loraNetDevice->GetMac ()->GetObject<EndDeviceLorawanMac>();
				mac->SetMaxNumberOfTransmissions (MAXRTX);
				mac->SetMType (LorawanMacHeader::CONFIRMED_DATA_UP);
			}
	}

  	/*********************
   	*  Create Gateways  *
   	*********************/

  	// Create the gateway nodes (allocate them uniformely on the disc)
  	NodeContainer gateways;
  	gateways.Create (nGateways);

    sAngle = (2*M_PI)/nGateways;  
   
  	Ptr<ListPositionAllocator> allocator = CreateObject<ListPositionAllocator> ();
  	// Make it so that nodes are at a certain height > 0
  	allocator->Add (Vector (0.0, 0.0, 0.0));
  	mobility.SetPositionAllocator (allocator);
  	mobility.Install (gateways);

  	// Make it so that nodes are at a certain height > 0
  	for (NodeContainer::Iterator j = gateways.Begin ();
    	j != gateways.End (); ++j){
      	Ptr<MobilityModel> mobility = (*j)->GetObject<MobilityModel> ();
      	Vector position = mobility->GetPosition ();
		position.x = gatewayRadius * cos(angle); 
  		position.y = gatewayRadius * sin(angle); 
      	position.z = 15;
      	mobility->SetPosition (position);
		angle += sAngle;
	}

  	// Create a netdevice for each gateway
  	phyHelper.SetDeviceType (LoraPhyHelper::GW);
  	macHelper.SetDeviceType (LorawanMacHelper::GW);
  	helper.Install (phyHelper, macHelper, gateways);

	/**********************
   	*  Handle buildings  *
   	**********************/
	buildingHandler(endDevices, gateways);	
 
  	/**********************************************
   	*  Set up the end device's spreading factor  *
   	**********************************************/

  	sfQuant = macHelper.SetSpreadingFactorsUp (endDevices, gateways, channel, flagRtx);
	//sfQuant = macHelper.SetSpreadingFactorsStrategies (endDevices, sfQuant, nRegulars, nDevices, LorawanMacHelper::SHIFT_ONE);
    //sfQuant = macHelper.SetSpreadingFactorsEIB (endDevices, radius);
    //sfQuant = macHelper.SetSpreadingFactorsEAB (endDevices, radius);
    //sfQuant = macHelper.SetSpreadingFactorsProp (endDevices, radius);

/*	cout << "SFs: ";
	for (int i=0; i< 6;i++)	
		cout << "  " << sfQuant.at(i);
	cout << endl;
*/

  	NS_LOG_DEBUG ("Completed configuration");

  	/*********************************************
   	*  Install applications on the end devices  *
   	*********************************************/

  	Time appStopTime = Seconds (simulationTime);

  	PeriodicSenderHelper appRegularHelper = PeriodicSenderHelper ();
  	appRegularHelper.SetPeriod (Seconds (appPeriodSeconds));
  	appRegularHelper.SetPacketSize (19);
  	ApplicationContainer appRegContainer = appRegularHelper.Install (endDevices.Get(0));
	for(int j = 1; j < nRegulars; j++)
		appRegContainer.Add(appRegularHelper.Install(endDevices.Get(j)));	

	NS_LOG_DEBUG("appRegulars -> startTime:" << 0 << "  stopTime:" << appStopTime.GetSeconds());
  	appRegContainer.Start (Seconds(0));
  	appRegContainer.Stop (appStopTime);

   	RandomSenderHelper appAlarmHelper = RandomSenderHelper ();
    appAlarmHelper.SetMean (appPeriodSeconds);
  	appAlarmHelper.SetPacketSize (5);
	ApplicationContainer appAlmContainer = appAlarmHelper.Install (endDevices.Get(nRegulars));
	for(int j = (nRegulars + 1); j < nDevices; j++)
		appAlmContainer.Add(appAlarmHelper.Install(endDevices.Get(j)));	

	NS_LOG_DEBUG("appAlarms -> startTime:" << 0 << "  stopTime:" << appStopTime.GetSeconds());
  	appAlmContainer.Start (Seconds(0));
  	appAlmContainer.Stop (appStopTime);

  	/**************************
   	*  Create Network Server  *
   	***************************/

  	// Create the NS node
  	NodeContainer networkServer;
  	networkServer.Create (1);

  	// Create a NS for the network
  	nsHelper.SetEndDevices (endDevices);
  	nsHelper.SetGateways (gateways);
  	nsHelper.Install (networkServer);

  	//Create a forwarder for each gateway
 	forHelper.Install (gateways);

 	/**********************
   	* Print output files *
   	*********************/
  	if (printEDs){
    	PrintEndDevices (endDevices, gateways, endDevRegFile, endDevAlmFile, gwFile); 	
 	}

  	////////////////
  	// Simulation //
  	////////////////

  	Simulator::Stop (appStopTime + Hours (1));

  	NS_LOG_INFO ("Running simulation...");
  	Simulator::Run ();

  	Simulator::Destroy ();

  	/////////////////////////////
  	// Metrics results to file //
  	////////////xx///////////////
  	
  	LoraPacketTracker &tracker = helper.GetPacketTracker ();

	if (sfQuant[0]){
    	NS_LOG_INFO (endl <<"//////////////////////////////////////////////");
    	NS_LOG_INFO ("//  Computing SF-7 performance metrics  //");
    	NS_LOG_INFO ("//////////////////////////////////////////////" << endl);

	  	NS_LOG_INFO ("--- Regulars performance metrics ---");
   	
		stringstream(tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1), REGULAR, (unsigned)nRegulars, (unsigned)nDevices, SF7)) >> sent >> received;

		stringstream(tracker.CountMacPacketsGloballyDelay (Seconds (0), appStopTime + Hours (1), (unsigned)nDevices, (unsigned)nGateways, SF7)) >> avgDelay;
	
		packLoss = sent - received;
  		throughput = received/simulationTime;

  		probSucc = received/sent;
  		probLoss = packLoss/sent;

		NS_LOG_INFO("----------------------------------------------------------------");
   		NS_LOG_INFO("nDevices" << "  |  " << "throughput" << "  |  "  << "probSucc"  << "  |  " << "probLoss" <<  "  |  " << "avgDelay"); 
   		NS_LOG_INFO(nDevices  << "       |  " << throughput << "    |  " << probSucc << "   |  " << probLoss << "   |  " << avgDelay);
		NS_LOG_INFO("----------------------------------------------------------------"<< endl);

  		myfile.open (fileSF7Metric, ios::out | ios::app);
  		myfile << nDevices << ", " << throughput << ", " << probSucc;
  	  	myfile.close();  

	   	NS_LOG_INFO("numDev:" << nDevices << " numGW:" << nGateways << " simTime:" << simulationTime << " throughput:" << throughput);
  		NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  		NS_LOG_INFO("sent:" << sent << "    succ:" << received << "     drop:"<< packLoss  << "   delay:" << avgDelay);
 	 	NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl);

 	 	myfile.open (fileData, ios::out | ios::app);
  		myfile << "sent: " << sent << " succ: " << received << " drop: "<< packLoss << "\n";
  		myfile << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << "\n";
  		myfile << "numRegDev: " << nRegulars << " numGat: " << nGateways << " simTime: " << simulationTime << " throughput: " << throughput<< "\n";
  		myfile << "##################################################################" << "\n\n";
  		myfile.close();  

	  	NS_LOG_INFO ("--- Alarms performance metrics ---");
   	
		stringstream(tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1), ALARM, (unsigned)nRegulars, (unsigned)nDevices, SF7)) >> sent >> received;

		stringstream(tracker.CountMacPacketsGloballyDelay (Seconds (0), appStopTime + Hours (1), (unsigned)nDevices, (unsigned)nGateways, SF7)) >> avgDelay;
	
		packLoss = sent - received;
  		throughput = received/simulationTime;

  		probSucc = received/sent;
  		probLoss = packLoss/sent;

		NS_LOG_INFO("----------------------------------------------------------------");
   		NS_LOG_INFO("nDevices" << "  |  " << "throughput" << "  |  "  << "probSucc"  << "  |  " << "probLoss" <<  "  |  " << "avgDelay"); 
   		NS_LOG_INFO(nDevices  << "       |  " << throughput << "    |  " << probSucc << "   |  " << probLoss << "   |  " << avgDelay);
		NS_LOG_INFO("----------------------------------------------------------------"<< endl);

  		myfile.open (fileSF7Metric, ios::out | ios::app);
  		myfile << ", " << throughput << ", " << probSucc << "\n";
  	  	myfile.close();  

	   	NS_LOG_INFO("numDev:" << nDevices << " numGW:" << nGateways << " simTime:" << simulationTime << " throughput:" << throughput);
  		NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  		NS_LOG_INFO("sent:" << sent << "    succ:" << received << "     drop:"<< packLoss  << "   delay:" << avgDelay);
 	 	NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl);

 	 	myfile.open (fileData, ios::out | ios::app);
  		myfile << "sent: " << sent << " succ: " << received << " drop: "<< packLoss << "\n";
  		myfile << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << "\n";
  		myfile << "numAlmgDev: " << nAlarms << " numGat: " << nGateways << " simTime: " << simulationTime << " throughput: " << throughput<< "\n";
  		myfile << "##################################################################" << "\n\n";
  		myfile.close();  

		//cout << "\nprintPhyPacketsPerGW:" <<endl;
		//for (int i=0; i<6;i++)
		//cout << tracker.CountPhyPacketsPerGw (Seconds (0), appStopTime + Hours (1), nDevices).at(i) << " ";
		//cout << endl;

		//for (int i=0; i<nGateways; i++)
  		//	cout << "\nprintPhyPacketsPerGW:\n"<< tracker.PrintPhyPacketsPerGw (Seconds (0), appStopTime + Hours (1), nDevices+i) << endl;

		//cout << "\ncountMacPAcketsGlobally:\n" << tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1)) << endl;

		//if (flagRtx){
		//	cout << "\ncountMacPAcketsGloballyCpsr:\n" << tracker.CountMacPacketsGloballyCpsr (Seconds (0), appStopTime + Hours (1)) << endl;
		//	tracker.PrintRetransmissions(Seconds(0), appStopTime + Hours(1), nDevices, MAXRTX);
		//}
	}

	if (sfQuant[1]){
    	NS_LOG_INFO (endl <<"//////////////////////////////////////////////");
    	NS_LOG_INFO ("//  Computing SF-8 performance metrics  //");
    	NS_LOG_INFO ("//////////////////////////////////////////////" << endl);

	  	NS_LOG_INFO ("--- Regulars performance metrics ---");
   	
		stringstream(tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1), REGULAR, (unsigned)nRegulars, (unsigned)nDevices, SF8)) >> sent >> received;

		stringstream(tracker.CountMacPacketsGloballyDelay (Seconds (0), appStopTime + Hours (1), (unsigned)nDevices, (unsigned)nGateways, SF8)) >> avgDelay;
	
		packLoss = sent - received;
  		throughput = received/simulationTime;

  		probSucc = received/sent;
  		probLoss = packLoss/sent;

		NS_LOG_INFO("----------------------------------------------------------------");
   		NS_LOG_INFO("nDevices" << "  |  " << "throughput" << "  |  "  << "probSucc"  << "  |  " << "probLoss" <<  "  |  " << "avgDelay"); 
   		NS_LOG_INFO(nDevices  << "       |  " << throughput << "    |  " << probSucc << "   |  " << probLoss << "   |  " << avgDelay);
		NS_LOG_INFO("----------------------------------------------------------------"<< endl);

  		myfile.open (fileSF8Metric, ios::out | ios::app);
  		myfile << nDevices << ", " << throughput << ", " << probSucc;
  	  	myfile.close();  

	   	NS_LOG_INFO("numDev:" << nDevices << " numGW:" << nGateways << " simTime:" << simulationTime << " throughput:" << throughput);
  		NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  		NS_LOG_INFO("sent:" << sent << "    succ:" << received << "     drop:"<< packLoss  << "   delay:" << avgDelay);
 	 	NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl);

 	 	myfile.open (fileData, ios::out | ios::app);
  		myfile << "sent: " << sent << " succ: " << received << " drop: "<< packLoss << "\n";
  		myfile << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << "\n";
  		myfile << "numRegDev: " << nRegulars << " numGat: " << nGateways << " simTime: " << simulationTime << " throughput: " << throughput<< "\n";
  		myfile << "##################################################################" << "\n\n";
  		myfile.close();  

	  	NS_LOG_INFO ("--- Alarms performance metrics ---");
   	
		stringstream(tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1), ALARM, (unsigned)nRegulars, (unsigned)nDevices, SF8)) >> sent >> received;

		stringstream(tracker.CountMacPacketsGloballyDelay (Seconds (0), appStopTime + Hours (1), (unsigned)nDevices, (unsigned)nGateways, SF8)) >> avgDelay;
	
		packLoss = sent - received;
  		throughput = received/simulationTime;

  		probSucc = received/sent;
  		probLoss = packLoss/sent;

		NS_LOG_INFO("----------------------------------------------------------------");
   		NS_LOG_INFO("nDevices" << "  |  " << "throughput" << "  |  "  << "probSucc"  << "  |  " << "probLoss" <<  "  |  " << "avgDelay"); 
   		NS_LOG_INFO(nDevices  << "       |  " << throughput << "    |  " << probSucc << "   |  " << probLoss << "   |  " << avgDelay);
		NS_LOG_INFO("----------------------------------------------------------------"<< endl);

  		myfile.open (fileSF8Metric, ios::out | ios::app);
  		myfile << ", " << throughput << ", " << probSucc << "\n";
  	  	myfile.close();  

	   	NS_LOG_INFO("numDev:" << nDevices << " numGW:" << nGateways << " simTime:" << simulationTime << " throughput:" << throughput);
  		NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  		NS_LOG_INFO("sent:" << sent << "    succ:" << received << "     drop:"<< packLoss  << "   delay:" << avgDelay);
 	 	NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl);

 	 	myfile.open (fileData, ios::out | ios::app);
  		myfile << "sent: " << sent << " succ: " << received << " drop: "<< packLoss << "\n";
  		myfile << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << "\n";
  		myfile << "numAlmgDev: " << nAlarms << " numGat: " << nGateways << " simTime: " << simulationTime << " throughput: " << throughput<< "\n";
  		myfile << "##################################################################" << "\n\n";
  		myfile.close();  

		//cout << "\nprintPhyPacketsPerGW:" <<endl;
		//for (int i=0; i<6;i++)
		//cout << tracker.CountPhyPacketsPerGw (Seconds (0), appStopTime + Hours (1), nDevices).at(i) << " ";
		//cout << endl;

		//for (int i=0; i<nGateways; i++)
  		//	cout << "\nprintPhyPacketsPerGW:\n"<< tracker.PrintPhyPacketsPerGw (Seconds (0), appStopTime + Hours (1), nDevices+i) << endl;

		//cout << "\ncountMacPAcketsGlobally:\n" << tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1)) << endl;

		//if (flagRtx){
		//	cout << "\ncountMacPAcketsGloballyCpsr:\n" << tracker.CountMacPacketsGloballyCpsr (Seconds (0), appStopTime + Hours (1)) << endl;
		//	tracker.PrintRetransmissions(Seconds(0), appStopTime + Hours(1), nDevices, MAXRTX);
		//}
	}

	if (sfQuant[2]){
    	NS_LOG_INFO (endl <<"//////////////////////////////////////////////");
    	NS_LOG_INFO ("//  Computing SF-9 performance metrics  //");
    	NS_LOG_INFO ("//////////////////////////////////////////////" << endl);

	  	NS_LOG_INFO ("--- Regulars performance metrics ---");
   	
		stringstream(tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1), REGULAR, (unsigned)nRegulars, (unsigned)nDevices, SF9)) >> sent >> received;

		stringstream(tracker.CountMacPacketsGloballyDelay (Seconds (0), appStopTime + Hours (1), (unsigned)nDevices, (unsigned)nGateways, SF9)) >> avgDelay;
	
		packLoss = sent - received;
  		throughput = received/simulationTime;

  		probSucc = received/sent;
  		probLoss = packLoss/sent;

		NS_LOG_INFO("----------------------------------------------------------------");
   		NS_LOG_INFO("nDevices" << "  |  " << "throughput" << "  |  "  << "probSucc"  << "  |  " << "probLoss" <<  "  |  " << "avgDelay"); 
   		NS_LOG_INFO(nDevices  << "       |  " << throughput << "    |  " << probSucc << "   |  " << probLoss << "   |  " << avgDelay);
		NS_LOG_INFO("----------------------------------------------------------------"<< endl);

  		myfile.open (fileSF9Metric, ios::out | ios::app);
  		myfile << nDevices << ", " << throughput << ", " << probSucc;
  	  	myfile.close();  

	   	NS_LOG_INFO("numDev:" << nDevices << " numGW:" << nGateways << " simTime:" << simulationTime << " throughput:" << throughput);
  		NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  		NS_LOG_INFO("sent:" << sent << "    succ:" << received << "     drop:"<< packLoss  << "   delay:" << avgDelay);
 	 	NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl);

 	 	myfile.open (fileData, ios::out | ios::app);
  		myfile << "sent: " << sent << " succ: " << received << " drop: "<< packLoss << "\n";
  		myfile << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << "\n";
  		myfile << "numRegDev: " << nRegulars << " numGat: " << nGateways << " simTime: " << simulationTime << " throughput: " << throughput<< "\n";
  		myfile << "##################################################################" << "\n\n";
  		myfile.close();  

	  	NS_LOG_INFO ("--- Alarms performance metrics ---");
   	
		stringstream(tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1), ALARM, (unsigned)nRegulars, (unsigned)nDevices, SF9)) >> sent >> received;

		stringstream(tracker.CountMacPacketsGloballyDelay (Seconds (0), appStopTime + Hours (1), (unsigned)nDevices, (unsigned)nGateways, SF9)) >> avgDelay;
	
		packLoss = sent - received;
  		throughput = received/simulationTime;

  		probSucc = received/sent;
  		probLoss = packLoss/sent;

		NS_LOG_INFO("----------------------------------------------------------------");
   		NS_LOG_INFO("nDevices" << "  |  " << "throughput" << "  |  "  << "probSucc"  << "  |  " << "probLoss" <<  "  |  " << "avgDelay"); 
   		NS_LOG_INFO(nDevices  << "       |  " << throughput << "    |  " << probSucc << "   |  " << probLoss << "   |  " << avgDelay);
		NS_LOG_INFO("----------------------------------------------------------------"<< endl);

  		myfile.open (fileSF9Metric, ios::out | ios::app);
  		myfile << ", " << throughput << ", " << probSucc << "\n";
  	  	myfile.close();  

	   	NS_LOG_INFO("numDev:" << nDevices << " numGW:" << nGateways << " simTime:" << simulationTime << " throughput:" << throughput);
  		NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
  		NS_LOG_INFO("sent:" << sent << "    succ:" << received << "     drop:"<< packLoss  << "   delay:" << avgDelay);
 	 	NS_LOG_INFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl);

 	 	myfile.open (fileData, ios::out | ios::app);
  		myfile << "sent: " << sent << " succ: " << received << " drop: "<< packLoss << "\n";
  		myfile << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << "\n";
  		myfile << "numAlmgDev: " << nAlarms << " numGat: " << nGateways << " simTime: " << simulationTime << " throughput: " << throughput<< "\n";
  		myfile << "##################################################################" << "\n\n";
  		myfile.close();  

		//cout << "\nprintPhyPacketsPerGW:" <<endl;
		//for (int i=0; i<6;i++)
		//cout << tracker.CountPhyPacketsPerGw (Seconds (0), appStopTime + Hours (1), nDevices).at(i) << " ";
		//cout << endl;

		//for (int i=0; i<nGateways; i++)
  		//	cout << "\nprintPhyPacketsPerGW:\n"<< tracker.PrintPhyPacketsPerGw (Seconds (0), appStopTime + Hours (1), nDevices+i) << endl;

		//cout << "\ncountMacPAcketsGlobally:\n" << tracker.CountMacPacketsGlobally (Seconds (0), appStopTime + Hours (1)) << endl;

		//if (flagRtx){
		//	cout << "\ncountMacPAcketsGloballyCpsr:\n" << tracker.CountMacPacketsGloballyCpsr (Seconds (0), appStopTime + Hours (1)) << endl;
		//	tracker.PrintRetransmissions(Seconds(0), appStopTime + Hours(1), nDevices, MAXRTX);
		//}
	}

  	return(0);
}

