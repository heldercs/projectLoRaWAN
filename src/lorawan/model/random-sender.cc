/*
 * =====================================================================================
 *
 *       Filename:  random-sender.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/01/2019 15:23:55
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder Candido (FHC), helderhdw@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include "ns3/random-sender.h"
#include "ns3/end-device-lorawan-mac.h"
#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/lora-net-device.h"

namespace ns3 {
namespace lorawan {


NS_LOG_COMPONENT_DEFINE ("RandomSender");

NS_OBJECT_ENSURE_REGISTERED (RandomSender);

TypeId RandomSender::GetTypeId (void){
	static TypeId tid = TypeId ("ns3::RandomSender")
    	.SetParent<Application> ()
    	.AddConstructor<RandomSender> ()
    	.SetGroupName ("lorawan")
    	.AddAttribute ("PacketSize", "The size of the packets this application sends, in bytes",
        	           StringValue ("ns3::ParetoRandomVariable[Bound=200,Shape=2.5]"),
            	       MakePointerAccessor (&RandomSender::m_pktSize),
                	   MakePointerChecker <RandomVariableStream>());
  	return(tid);
}

RandomSender::RandomSender () :
	m_initialDelay (Seconds (1)),
	m_randomPktSize (0),
  	m_basePktSize (5){
	NS_LOG_FUNCTION_NOARGS ();
	
	m_nextDelay = CreateObject<ExponentialRandomVariable> ();
	m_nextDelay->SetAttribute ("Mean", DoubleValue (10));
	//m_nextDelay->SetAttribute ("Bound", DoubleValue (10));
}

RandomSender::~RandomSender (){
	NS_LOG_FUNCTION_NOARGS ();
}

void RandomSender::SetInitialDelay (Time delay){
  NS_LOG_FUNCTION (this << delay);
  m_initialDelay = delay;
}

void RandomSender::SetMean (double mean){
  NS_LOG_FUNCTION (this << mean);
  m_nextDelay->SetAttribute ("Mean", DoubleValue(mean));
}

void RandomSender::SetBound (double bound){
  NS_LOG_FUNCTION (this << bound);
  m_nextDelay->SetAttribute ("Bound", DoubleValue(bound));
}

void RandomSender::SetPacketSize (uint8_t size){
  m_basePktSize = size;
}

void RandomSender::SendPacket (void){
  	NS_LOG_FUNCTION (this);

	Time nxtDelay;
  	// Create and send a new packet
  	int size = m_pktSize->GetInteger ();
  	Ptr<Packet> packet;
  	if (m_randomPktSize == true){
      	packet = Create<Packet>(m_basePktSize+size);
    }else{
      	packet = Create<Packet>(m_basePktSize);
    }

	m_mac->Send (packet);
	
    nxtDelay = Seconds(m_nextDelay->GetValue()); 
 	//NS_LOG_DEBUG("nxt: " << nxtDelay.GetSeconds() << " now: " << Simulator::Now().GetSeconds()); 
	NS_LOG_DEBUG ("The next event with a = " <<
                nxtDelay.GetSeconds() << " Seconds delay");
	
	// Schedule the next SendPacket event
  	m_sendEvent = Simulator::Schedule (nxtDelay, &RandomSender::SendPacket, this);

  	NS_LOG_DEBUG ("Sent a packet of size " << packet->GetSize ());
}

void RandomSender::StartApplication (void){
	NS_LOG_FUNCTION (this);

  	// Make sure we have a MAC layer
  	if (m_mac == 0){
      	// Assumes there's only one device
      	Ptr<LoraNetDevice> loraNetDevice = m_node->GetDevice (0)->GetObject<LoraNetDevice> ();

      	m_mac = loraNetDevice->GetMac ();
      	NS_ASSERT (m_mac != 0);
    }

	// Schedule the next SendPacket event
  	Simulator::Cancel (m_sendEvent);
  	NS_LOG_DEBUG ("Starting up application with a first event with a " <<
     	           m_initialDelay.GetSeconds () << " seconds delay");
  	m_sendEvent = Simulator::Schedule (m_initialDelay,
                                     	&RandomSender::SendPacket, this);
  	NS_LOG_DEBUG ("Event Id: " << m_sendEvent.GetUid ());
}

void RandomSender::StopApplication (void){
  	NS_LOG_FUNCTION_NOARGS ();
  	Simulator::Cancel (m_sendEvent);
}

}
}
