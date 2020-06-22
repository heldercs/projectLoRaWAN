/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 University of Padova
 *
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
 *
 * Author: Davide Magrin <magrinda@dei.unipd.it>
 */

#include "lora-packet-tracker.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/lorawan-mac-header.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace ns3 {
namespace lorawan {
NS_LOG_COMPONENT_DEFINE ("LoraPacketTracker");

LoraPacketTracker::LoraPacketTracker ()
{
  NS_LOG_FUNCTION (this);
}

LoraPacketTracker::~LoraPacketTracker ()
{
  NS_LOG_FUNCTION (this);
}

/////////////////
// MAC metrics //
/////////////////

void
LoraPacketTracker::MacTransmissionCallback (Ptr<Packet const> packet, uint8_t sf)
{
  if (IsUplink (packet))
    {
      NS_LOG_INFO ("A new packet was sent by the MAC layer");

      MacPacketStatus status;
      status.packet = packet;
      status.sendTime = Simulator::Now ();
      status.senderId = Simulator::GetContext ();
      status.receivedTime = Time::Max ();
	  status.sf = sf;
	
	  //cout << "A new packet was sent by id: " << status.senderId << " sendTime: " << status.sendTime.GetSeconds() << endl; 
      m_macPacketTracker.insert (std::pair<Ptr<Packet const>, MacPacketStatus>
                                   (packet, status));
    }
}

void
LoraPacketTracker::RequiredTransmissionsCallback (uint8_t reqTx, bool success,
                                                  Time firstAttempt,
                                                  Ptr<Packet> packet)
{
  NS_LOG_INFO ("Finished retransmission attempts for a packet");
  NS_LOG_DEBUG ("Packet: " << packet << "ReqTx " << unsigned(reqTx) <<
                ", succ: " << success << ", firstAttempt: " <<
                firstAttempt.GetSeconds ());
  
  RetransmissionStatus entry;
  entry.firstAttempt = firstAttempt;
  entry.finishTime = Simulator::Now ();
  entry.reTxAttempts = reqTx;
  entry.successful = success;

  //cout << "finished retransmission by id: " << Simulator::GetContext() << " succ: " << entry.successful << " frtTime: " << entry.firstAttempt.GetSeconds() << " finishTime: " << entry.finishTime.GetSeconds() << " ReqTx: " << (unsigned)entry.reTxAttempts << endl; 

  m_reTransmissionTracker.insert (std::pair<Ptr<Packet>, RetransmissionStatus>
                                    (packet, entry));
}

void
LoraPacketTracker::MacGwReceptionCallback (Ptr<Packet const> packet)
{
  if (IsUplink (packet))
    {
      NS_LOG_INFO ("A packet was successfully received" <<
                   " at the MAC layer of gateway " <<
                   Simulator::GetContext ());

      // Find the received packet in the m_macPacketTracker
      auto it = m_macPacketTracker.find (packet);

	  //cout << "A packet was success received id: " << (*it).second.senderId << " rcvTime: " << (*it).second.receivedTime.GetSeconds() << endl; 

      if (it != m_macPacketTracker.end ())
        {
          (*it).second.receptionTimes.insert (std::pair<int, Time>
                                                (Simulator::GetContext (),
                                                Simulator::Now ()));
        }
      else
        {
          NS_ABORT_MSG ("Packet not found in tracker");
        }
    }
}

/////////////////
// PHY metrics //
/////////////////

void
LoraPacketTracker::TransmissionCallback (Ptr<Packet const> packet, uint32_t edId)
{
  if (IsUplink (packet))
    {
      NS_LOG_INFO ("PHY packet " << packet
                                 << " was transmitted by device "
                                 << edId);
      // Create a packetStatus
      PacketStatus status;
      status.packet = packet;
      status.sendTime = Simulator::Now ();
      status.senderId = edId;

      m_packetTracker.insert (std::pair<Ptr<Packet const>, PacketStatus> (packet, status));
    }
}

void
LoraPacketTracker::PacketReceptionCallback (Ptr<Packet const> packet, uint32_t gwId)
{
  if (IsUplink (packet))
    {
      // Remove the successfully received packet from the list of sent ones
      NS_LOG_INFO ("PHY packet " << packet
                                 << " was successfully received at gateway "
                                 << gwId);


      std::map<Ptr<Packet const>, PacketStatus>::iterator it = m_packetTracker.find (packet);
      (*it).second.outcomes.insert (std::pair<int, enum PhyPacketOutcome> (gwId,
                                                                           RECEIVED));
    }
}

void
LoraPacketTracker::InterferenceCallback (Ptr<Packet const> packet, uint32_t gwId)
{
  if (IsUplink (packet))
    {
      NS_LOG_INFO ("PHY packet " << packet
                                 << " was interfered at gateway "
                                 << gwId);

      std::map<Ptr<Packet const>, PacketStatus>::iterator it = m_packetTracker.find (packet);
      (*it).second.outcomes.insert (std::pair<int, enum PhyPacketOutcome> (gwId,
                                                                           INTERFERED));
    }
}

void
LoraPacketTracker::NoMoreReceiversCallback (Ptr<Packet const> packet, uint32_t gwId)
{
  if (IsUplink (packet))
    {
      NS_LOG_INFO ("PHY packet " << packet
                                 << " was lost because no more receivers at gateway "
                                 << gwId);
      std::map<Ptr<Packet const>, PacketStatus>::iterator it = m_packetTracker.find (packet);
      (*it).second.outcomes.insert (std::pair<int, enum PhyPacketOutcome> (gwId,
                                                                           NO_MORE_RECEIVERS));
    }
}

void
LoraPacketTracker::UnderSensitivityCallback (Ptr<Packet const> packet, uint32_t gwId)
{
  if (IsUplink (packet))
    {
      NS_LOG_INFO ("PHY packet " << packet
                                 << " was lost because under sensitivity at gateway "
                                 << gwId);

      std::map<Ptr<Packet const>, PacketStatus>::iterator it = m_packetTracker.find (packet);
      (*it).second.outcomes.insert (std::pair<int, enum PhyPacketOutcome> (gwId,
                                                                           UNDER_SENSITIVITY));
    }
}

void
LoraPacketTracker::LostBecauseTxCallback (Ptr<Packet const> packet, uint32_t gwId)
{
  if (IsUplink (packet))
    {
      NS_LOG_INFO ("PHY packet " << packet
                                 << " was lost because of GW transmission at gateway "
                                 << gwId);

      std::map<Ptr<Packet const>, PacketStatus>::iterator it = m_packetTracker.find (packet);
      (*it).second.outcomes.insert (std::pair<int, enum PhyPacketOutcome> (gwId,
                                                                           LOST_BECAUSE_TX));
    }
}

bool
LoraPacketTracker::IsUplink (Ptr<Packet const> packet)
{
  NS_LOG_FUNCTION (this);

  LorawanMacHeader mHdr;
  Ptr<Packet> copy = packet->Copy ();
  copy->RemoveHeader (mHdr);
  return mHdr.IsUplink ();
}

////////////////////////
// Counting Functions //
////////////////////////

void
LoraPacketTracker::PrintVector (std::vector<int> vector)
{
  // NS_LOG_INFO ("PrintRetransmissions");

  for (int i = 0; i < int(vector.size ()); i++)
    {
      // NS_LOG_INFO ("i: " << i);
      cout << vector.at (i) << " ";
    }
  //
  // cout << endl;
}


void
LoraPacketTracker::PrintSumRetransmissions (std::vector<int> reTxVector)
{
  // NS_LOG_INFO ("PrintSumRetransmissions");

  int total = 0;
  for (int i = 0; i < int(reTxVector.size ()); i++)
    {
      // NS_LOG_INFO ("i: " << i);
      total += reTxVector[i] * (i + 1);
    }
  cout << total;
}



void
LoraPacketTracker::PrintRetransmissions (Time startTime, Time stopTime, int gwId, uint8_t mRtx){
  std::vector<int> totalReTxAmounts (mRtx, 0);
  std::vector<int> successfulReTxAmounts (mRtx, 0);
  std::vector<int> failedReTxAmounts (mRtx, 0);
  Time delaySum = Seconds (0);
  Time ackDelaySum = Seconds (0);

  int packetsOutsideTransient = 0;
  int MACpacketsOutsideTransient = 0;

  for (auto itMac = m_macPacketTracker.begin (); itMac != m_macPacketTracker.end (); ++itMac)
    {
      // NS_LOG_DEBUG ("Dealing with packet " << (*itMac).first);

      if ((*itMac).second.sendTime > startTime && (*itMac).second.sendTime < stopTime)
        {
          // Count retransmissions
          ////////////////////////
          auto itRetx = m_reTransmissionTracker.find ((*itMac).first);

          if (itRetx == m_reTransmissionTracker.end ())
            {
              // This means that the device did not finish retransmitting
              NS_ABORT_MSG ("Searched packet was not found" << "Packet " <<
                            (*itMac).first << " not found. Sent at " <<
                            (*itMac).second.sendTime.GetSeconds ());
            }

          packetsOutsideTransient++;
          MACpacketsOutsideTransient++;
          totalReTxAmounts.at ((*itRetx).second.reTxAttempts - 1)++;

          if ((*itRetx).second.successful)
            {
              successfulReTxAmounts.at ((*itRetx).second.reTxAttempts - 1)++;
            }
          else
            {
              failedReTxAmounts.at ((*itRetx).second.reTxAttempts - 1)++;
            }
		  
          // Compute delays
          /////////////////
          if ((*itMac).second.receptionTimes.find(Simulator::GetContext ())->second == Time::Max ())
            {
              // NS_LOG_DEBUG ("Packet never received, ignoring it");
              packetsOutsideTransient--;
            }
          else
            {
              delaySum += (*itMac).second.receptionTimes.find(Simulator::GetContext ())->second  - (*itMac).second.sendTime;
              ackDelaySum += (*itRetx).second.finishTime - (*itRetx).second.firstAttempt;
            }

        }
    }

  // Sum PHY outcomes
  //////////////////////////////////
  // vector performanceAmounts will contain - for the interval given in the
  // input of the function, the following fields:
  // totPacketsSent receivedPackets interferedPackets noMoreGwPackets underSensitivityPackets
  std::vector<int> performancesAmounts (6, 0);
  for (auto itPhy = m_packetTracker.begin (); itPhy != m_packetTracker.end (); ++itPhy)
    {
      if ((*itPhy).second.sendTime >= startTime && (*itPhy).second.sendTime <= stopTime)
        {
          performancesAmounts.at (0)++;
          if ((*itPhy).second.outcomes.count (gwId) > 0){
	          switch ((*itPhy).second.outcomes.at(gwId))
    	      {
       			case RECEIVED:
              	{
                	performancesAmounts.at (1)++;
                	break;
             	}
            	case INTERFERED:
              	{
                	performancesAmounts.at (2)++;
                	break;
              	}
            	case NO_MORE_RECEIVERS:
              	{
                	performancesAmounts.at (3)++;
                	break;
              	}
            	case UNDER_SENSITIVITY:
              	{
                	performancesAmounts.at (4)++;
                	break;
              	}
            	case LOST_BECAUSE_TX:
              	{
                	performancesAmounts.at (5)++;
                	break;
              	}
            	case UNSET:
              	{
                	break;
              	}
              }     //end switch
		  }
        }
    }

  //cout << "pckOutTrans: " << packetsOutsideTransient++ << " macPckOutTrans: " << MACpacketsOutsideTransient++ << endl;
  //cout << "delay: " << delaySum.GetSeconds() << " ackDelay: " << ackDelaySum.GetSeconds() << endl;

  double avgDelay = 0;
  double avgAckDelay = 0;
  if (packetsOutsideTransient != 0)
    {
      avgDelay = (delaySum / packetsOutsideTransient).GetSeconds ();
      avgAckDelay = ((ackDelaySum) / packetsOutsideTransient).GetSeconds ();
    }

  // Print PHY
  cout << std::endl << "PHY" << std::endl << "---" << endl;

  std::vector<std::string> meanings = {"TOT:  ", "SUCC: ", "INT:  ", "NMR:  ", "US:   ", "GWTX: "};
  for (int i = 0; i < int(performancesAmounts.size ()); i++)
    {
      // NS_LOG_INFO ("i: " << i);
      cout << meanings.at(i) << "\t" << performancesAmounts.at (i) << endl;
    }

  cout << endl;

  // Print MAC packet data
  cout << "MAC Confirmed" << std::endl << "-------------" << endl;

  cout << "Successes: "; PrintVector (successfulReTxAmounts); cout << endl;
  cout << "Failures:  "; PrintVector (failedReTxAmounts); cout << endl;
  cout << "Delays:    " << avgDelay << " " << avgAckDelay << endl;
  cout << "Totals:    "; PrintSumRetransmissions (totalReTxAmounts); cout << endl;
}


std::vector<int>
LoraPacketTracker::CountPhyPacketsPerGw (Time startTime, Time stopTime,
                                         int gwId)
{
  // Vector packetCounts will contain - for the interval given in the input of
  // the function, the following fields: totPacketsSent receivedPackets
  // interferedPackets noMoreGwPackets underSensitivityPackets

  std::vector<int> packetCounts (6, 0);

  for (auto itPhy = m_packetTracker.begin ();
       itPhy != m_packetTracker.end ();
       ++itPhy)
    {
      if ((*itPhy).second.sendTime >= startTime && (*itPhy).second.sendTime <= stopTime)
        {
          packetCounts.at (0)++;

          NS_LOG_DEBUG ("Dealing with packet " << (*itPhy).second.packet);
          NS_LOG_DEBUG ("This packet was received by " <<
                        (*itPhy).second.outcomes.size () << " gateways");

          if ((*itPhy).second.outcomes.count (gwId) > 0)
            {
              switch ((*itPhy).second.outcomes.at (gwId))
                {
                case RECEIVED:
                  {
                    packetCounts.at (1)++;
                    break;
                  }
                case INTERFERED:
                  {
                    packetCounts.at (2)++;
                    break;
                  }
                case NO_MORE_RECEIVERS:
                  {
                    packetCounts.at (3)++;
                    break;
                  }
                case UNDER_SENSITIVITY:
                  {
                    packetCounts.at (4)++;
                    break;
                  }
                case LOST_BECAUSE_TX:
                  {
                    packetCounts.at (5)++;
                    break;
                  }
                case UNSET:
                  {
                    break;
                  }
                }
            }
        }
    }

  return packetCounts;
}
std::string
LoraPacketTracker::PrintPhyPacketsPerGw (Time startTime, Time stopTime,
                                         int gwId)
{
  // Vector packetCounts will contain - for the interval given in the input of
  // the function, the following fields: totPacketsSent receivedPackets
  // interferedPackets noMoreGwPackets underSensitivityPackets

  std::vector<int> packetCounts (6, 0);

  for (auto itPhy = m_packetTracker.begin ();
       itPhy != m_packetTracker.end ();
       ++itPhy)
    {
      if ((*itPhy).second.sendTime >= startTime && (*itPhy).second.sendTime <= stopTime)
        {
          packetCounts.at (0)++;

          NS_LOG_DEBUG ("Dealing with packet " << (*itPhy).second.packet);
          NS_LOG_DEBUG ("This packet was received by " <<
                        (*itPhy).second.outcomes.size () << " gateways");

          if ((*itPhy).second.outcomes.count (gwId) > 0)
            {
              switch ((*itPhy).second.outcomes.at (gwId))
                {
                case RECEIVED:
                  {
                    packetCounts.at (1)++;
                    break;
                  }
                case INTERFERED:
                  {
                    packetCounts.at (2)++;
                    break;
                  }
                case NO_MORE_RECEIVERS:
                  {
                    packetCounts.at (3)++;
                    break;
                  }
                case UNDER_SENSITIVITY:
                  {
                    packetCounts.at (4)++;
                    break;
                  }
                case LOST_BECAUSE_TX:
                  {
                    packetCounts.at (5)++;
                    break;
                  }
                case UNSET:
                  {
                    break;
                  }
                }
            }
        }
    }

  std::string output ("");
  for (int i = 0; i < 6; ++i)
    {
		switch (i) {
				case 0:
						output += "SENT: ";
						break;
				case RECEIVED+1:
						output += "REC: ";
						break;
				case INTERFERED+1:
						output += "INTER: ";
						break;
				case NO_MORE_RECEIVERS+1:
						output += "NO_REC: ";
						break;
  				case UNDER_SENSITIVITY+1:
						output += "USENS: ";
						break;
				case LOST_BECAUSE_TX+1:
						output += "LOST_TX: ";
				default:	
						break;
		}/* -----  end switch  ----- */
      	output += std::to_string (packetCounts.at (i)) + "\n";
    }

  return output;
}

  std::string
  LoraPacketTracker::CountMacPacketsGlobally (Time startTime, Time stopTime)
  {
    NS_LOG_FUNCTION (this << startTime << stopTime);

  	double sent = 0;
  	double received = 0;
 
    for (auto it = m_macPacketTracker.begin ();
         it != m_macPacketTracker.end ();
         ++it)
      {
		//cout << "id: " << (*it).second.senderId << endl;
        if ((*it).second.sendTime >= startTime && (*it).second.sendTime <= stopTime)
          {
            sent++;
            if ((*it).second.receptionTimes.size ())
              {
              	received++;
              }
          }
      }

    return std::to_string (sent) + " " +
      std::to_string (received);
  }

  std::string
  LoraPacketTracker::CountMacPacketsGlobally (Time startTime, Time stopTime, uint8_t sf)
  {
    NS_LOG_FUNCTION (this << startTime << stopTime);

  	double sent = 0;
  	double received = 0;
 
    for (auto it = m_macPacketTracker.begin ();
         it != m_macPacketTracker.end ();
         ++it)
    {
		//cout << "id: " << (*it).second.senderId << endl;
		if ((*it).second.sf == sf)
		{
        	if ((*it).second.sendTime >= startTime && (*it).second.sendTime <= stopTime)
          	{
            	sent++;
            	if ((*it).second.receptionTimes.size ())
              	{
              		received++;
              	}
          	}
      	}
	}
    return std::to_string (sent) + " " +
      std::to_string (received);
  }

  std::string
  LoraPacketTracker::CountMacPacketsGlobally (Time startTime, Time stopTime, bool nodeType, uint32_t nodeEdge, uint32_t nDev)
  {
    NS_LOG_FUNCTION (this << startTime << stopTime);

  	double sent = 0;
  	double received = 0;
 
    for (auto it = m_macPacketTracker.begin ();
         it != m_macPacketTracker.end ();
         ++it)
      {
		if (nodeType){
			if ((*it).second.senderId >= 0 && (*it).second.senderId < nodeEdge){
        		if ((*it).second.sendTime >= startTime && (*it).second.sendTime <= stopTime)
          		{
            		sent++;
            		if ((*it).second.receptionTimes.size ())
              		{
             	  	 	received++;
             		}
          		}
			}
		}else{
    		if ((*it).second.senderId >= nodeEdge && (*it).second.senderId < nDev){
        		if ((*it).second.sendTime >= startTime && (*it).second.sendTime <= stopTime)
          		{
            		sent++;
            		if ((*it).second.receptionTimes.size ())
              		{
                		received++;
             		}
          		}
			}
		}
      
	  }

    return std::to_string (sent) + " " +
      std::to_string (received);
  }


  std::string
  LoraPacketTracker::CountMacPacketsGloballyCpsr (Time startTime, Time stopTime)
  {
    NS_LOG_FUNCTION (this << startTime << stopTime);

  	double successfulReTxAmounts = 0;
  	uint32_t totalReTxAmounts = 0;
    Time sumDelay = Seconds(0);
  	double avgDelay = 0;
	int packetsOutsideTransient = 0;

  	for (auto itMac = m_macPacketTracker.begin (); itMac != m_macPacketTracker.end (); ++itMac)
    {
      // NS_LOG_DEBUG ("Dealing with packet " << (*itMac).first);

      if ((*itMac).second.sendTime > startTime && (*itMac).second.sendTime < stopTime)
        {
          // Count retransmissions
          ////////////////////////
          auto itRetx = m_reTransmissionTracker.find ((*itMac).first);

          if (itRetx == m_reTransmissionTracker.end ())
            {
              // This means that the device did not finish retransmitting
              NS_ABORT_MSG ("Searched packet was not found" << "Packet " <<
                            (*itMac).first << " not found. Sent at " <<
                            (*itMac).second.sendTime.GetSeconds ());
            }

          packetsOutsideTransient++;
          totalReTxAmounts += (*itRetx).second.reTxAttempts;

          if ((*itRetx).second.successful)
            {
              successfulReTxAmounts++;
            }
		  
          // Compute delays
          /////////////////
          if ((*itMac).second.receptionTimes.find(Simulator::GetContext ())->second == Time::Max ())
            {
              // NS_LOG_DEBUG ("Packet never received, ignoring it");
              //cout << "Packet never received, ignoring it" << endl;;
              packetsOutsideTransient--;
            }
          else
            {
			  //cout << "rcpT: " << (*itMac).second.receptionTimes.find(Simulator::GetContext ())->second << endl;
              sumDelay += (*itRetx).second.finishTime - (*itRetx).second.firstAttempt;
            }

        }
    }

	//cout << "numRTX: " <<  (unsigned)totalReTxAmounts << endl;
	
	totalReTxAmounts -= packetsOutsideTransient; 

	//cout << "delay: " << sumDelay.GetSeconds() << endl;
	
  	if (packetsOutsideTransient != 0)
    {
      avgDelay = (sumDelay/packetsOutsideTransient).GetSeconds ();
    }

	//cout << "sent: " << packetsOutsideTransient << " rec: " << successfulReTxAmounts << " nRtx: " << (unsigned)totalReTxAmounts << " delay: " << avgDelay << endl;
	
    return std::to_string(avgDelay); 
  }

  std::string
  LoraPacketTracker::CountMacPacketsGloballyCpsr (Time startTime, Time stopTime, bool nodeType, uint32_t nodeEdge, uint32_t nDev)
  {
    NS_LOG_FUNCTION (this << startTime << stopTime);

  	double successfulReTxAmounts = 0;
  	uint32_t totalReTxAmounts = 0;
    Time sumDelay = Seconds(0);
  	double avgDelay = 0;
	int packetsOutsideTransient = 0;

  	for (auto itMac = m_macPacketTracker.begin (); itMac != m_macPacketTracker.end (); ++itMac)
    {
      // NS_LOG_DEBUG ("Dealing with packet " << (*itMac).first);

	  if(!nodeType){
		if ((*itMac).second.senderId >= nodeEdge && (*itMac).second.senderId < nDev){
      		if ((*itMac).second.sendTime > startTime && (*itMac).second.sendTime < stopTime)
       		{
        	  	// Count retransmissions
          		////////////////////////
          		auto itRetx = m_reTransmissionTracker.find ((*itMac).first);

          		if (itRetx == m_reTransmissionTracker.end ())
            	{
              		// This means that the device did not finish retransmitting
            	 	 NS_ABORT_MSG ("Searched packet was not found" << "Packet " <<
                		            (*itMac).first << " not found. Sent at " <<
            	   	    	         (*itMac).second.sendTime.GetSeconds ());
            	}

          		packetsOutsideTransient++;
          		totalReTxAmounts += (*itRetx).second.reTxAttempts;

          		if ((*itRetx).second.successful)
            	{
           	   		successfulReTxAmounts++;
            	}
		  
          		// Compute delays
          		/////////////////
          		if ((*itMac).second.receptionTimes.find(Simulator::GetContext ())->second == Time::Max ())
            	{
            		  // NS_LOG_DEBUG ("Packet never received, ignoring it");
             		 cout << "Packet never received, ignoring it" << endl;;
             	 	packetsOutsideTransient--;
            	}
          		else
            	{
              	sumDelay += (*itRetx).second.finishTime - (*itRetx).second.firstAttempt;
            	}

          	}
		}
	  }
    }

	//cout << "numRTX: " <<  (unsigned)totalReTxAmounts << endl;
	
	totalReTxAmounts -= packetsOutsideTransient; 

	//cout << "delay: " << sumDelay.GetSeconds() << endl;
	
  	if (packetsOutsideTransient != 0)
    {
      avgDelay = (sumDelay/packetsOutsideTransient).GetSeconds ();
    }

	//cout << "sent: " << packetsOutsideTransient << " rec: " << successfulReTxAmounts << " nRtx: " << (unsigned)totalReTxAmounts << " delay: " << avgDelay << endl;
	
    return std::to_string(avgDelay); 
  }

  std::string
  LoraPacketTracker::CountMacPacketsGloballyDelay (Time startTime, Time stopTime, uint32_t gwId, uint32_t gwNum)
  {
  	Time delaySum = Seconds (0);
  	double avgDelay = 0;
  	int packetsOutsideTransient = 0;

	for (uint32_t i = gwId; i < (gwId+gwNum); i++)
	{
  		for (auto itMac = m_macPacketTracker.begin (); itMac != m_macPacketTracker.end (); ++itMac)
    	{
      	// NS_LOG_DEBUG ("Dealing with packet " << (*itMac).first);

      		if ((*itMac).second.sendTime > startTime && (*itMac).second.sendTime < stopTime)
        	{
          		packetsOutsideTransient++;

          		// Compute delays
          		/////////////////
          		if ((*itMac).second.receptionTimes.find(gwId)->second == Time::Max () || (*itMac).second.receptionTimes.find(gwId)->second < (*itMac).second.sendTime )
            	{
              		// NS_LOG_DEBUG ("Packet never received, ignoring it");
              		packetsOutsideTransient--;
           	 	}
          		else
            	{
              		delaySum += (*itMac).second.receptionTimes.find(gwId)->second  - (*itMac).second.sendTime;
            	}

        	}
    	}
	}
	//cout << "trans: " << packetsOutsideTransient << " d: " << delaySum.GetSeconds() << endl;

  	if (packetsOutsideTransient != 0)
    {
      avgDelay = (delaySum/packetsOutsideTransient).GetSeconds ();
    }
	
	return(std::to_string(avgDelay));

}

  std::string
  LoraPacketTracker::CountMacPacketsGloballyDelay (Time startTime, Time stopTime, uint32_t gwId, uint32_t gwNum, uint8_t sf)
  {
  	Time delaySum = Seconds (0);
  	double avgDelay = 0;
  	int packetsOutsideTransient = 0;

	for (uint32_t i = gwId; i < (gwId+gwNum); i++)
	{
  		for (auto itMac = m_macPacketTracker.begin (); itMac != m_macPacketTracker.end (); ++itMac)
    	{
      	// NS_LOG_DEBUG ("Dealing with packet " << (*itMac).first);
			if ((*itMac).second.sf == sf){
      			if ((*itMac).second.sendTime > startTime && (*itMac).second.sendTime < stopTime)
        		{
          			packetsOutsideTransient++;

          			// Compute delays
          			/////////////////
          			if ((*itMac).second.receptionTimes.find(gwId)->second == Time::Max () || (*itMac).second.receptionTimes.find(gwId)->second < (*itMac).second.sendTime )
            		{
              			// NS_LOG_DEBUG ("Packet never received, ignoring it");
              			packetsOutsideTransient--;
           	 		}
          			else
            		{
              			delaySum += (*itMac).second.receptionTimes.find(gwId)->second  - (*itMac).second.sendTime;
            		}

        		}
			}
    	}
	}
	//cout << "trans: " << packetsOutsideTransient << " d: " << delaySum.GetSeconds() << endl;

  	if (packetsOutsideTransient != 0)
    {
      avgDelay = (delaySum/packetsOutsideTransient).GetSeconds ();
    }
	
	return(std::to_string(avgDelay));

}
  std::string
  LoraPacketTracker::CountMacPacketsGloballyDelay (Time startTime, Time stopTime, bool nodeType, uint32_t nodeEdge, uint32_t gwId, uint32_t gwNum)
  {
  	Time delaySum = Seconds (0);
  	double avgDelay = 0;
  	int packetsOutsideTransient = 0;

	for (uint32_t i = gwId; i < (gwId+gwNum); i++)
	{
  		for (auto itMac = m_macPacketTracker.begin (); itMac != m_macPacketTracker.end (); ++itMac)
    	{
      	// NS_LOG_DEBUG ("Dealing with packet " << (*itMac).first);
			if (nodeType){
				if ((*itMac).second.senderId >= 0 && (*itMac).second.senderId < nodeEdge){
      				if ((*itMac).second.sendTime > startTime && (*itMac).second.sendTime < stopTime)
        			{
    	      			packetsOutsideTransient++;

        	  			// Compute delays
          				/////////////////
 		          		if ((*itMac).second.receptionTimes.find(gwId)->second == Time::Max () || 
							(*itMac).second.receptionTimes.find(gwId)->second < (*itMac).second.sendTime ||
							(*itMac).second.receptionTimes.find(gwId)->second.GetSeconds() - (*itMac).second.sendTime.GetSeconds() > 10 )
            			{
              				// NS_LOG_DEBUG ("Packet never received, ignoring it");
              				packetsOutsideTransient--;
           	 			}
          				else
            			{
              				delaySum += (*itMac).second.receptionTimes.find(gwId)->second  - (*itMac).second.sendTime;
 	          			}
        			}
				}
			}
			else
			{
	    		if ((*itMac).second.senderId >= nodeEdge && (*itMac).second.senderId < gwId){
 					if ((*itMac).second.sendTime > startTime && (*itMac).second.sendTime < stopTime)
        			{
    	      			packetsOutsideTransient++;
        	  			// Compute delays
          				/////////////////
    		          	if ((*itMac).second.receptionTimes.find(gwId)->second == Time::Max () || (*itMac).second.receptionTimes.find(gwId)->second < (*itMac).second.sendTime )
           				{
              				// NS_LOG_DEBUG ("Packet never received, ignoring it");
              				packetsOutsideTransient--;
           	 			}
          				else
            			{
              				delaySum += (*itMac).second.receptionTimes.find(gwId)->second  - (*itMac).second.sendTime;
            			}
        			}
				}
			}
    	}
	}
	//cout << "Trans: " << packetsOutsideTransient << " d: " << delaySum.GetSeconds() << endl;

  	if (packetsOutsideTransient != 0)
    {
      avgDelay = (delaySum/packetsOutsideTransient).GetSeconds ();
    }
	
	return(std::to_string(avgDelay));

}



}
}
