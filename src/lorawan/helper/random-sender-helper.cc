/*
 * =====================================================================================
 *
 *       Filename:  random-sender-helper.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/01/2019 15:24:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder Candido (FHC), helderhdw@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#include "ns3/random-sender-helper.h"
#include "ns3/random-variable-stream.h"
#include "ns3/random-sender.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

namespace ns3 {
namespace lorawan{

NS_LOG_COMPONENT_DEFINE ("RandomSenderHelper");

RandomSenderHelper::RandomSenderHelper (){
	m_factory.SetTypeId ("ns3::RandomSender");

	m_factory.Set ("PacketSize", StringValue
                   ("ns3::ParetoRandomVariable[Bound=10|Shape=2.5]"));

	m_initialDelay = CreateObject<ExponentialRandomVariable> ();
	m_initialDelay->SetAttribute ("Mean", DoubleValue (10));
	//m_initialDelay->SetAttribute ("Bound", DoubleValue (10));

  	m_pktSize = 5;
}

RandomSenderHelper::~RandomSenderHelper (){
}

void RandomSenderHelper::SetAttribute (std::string name, const AttributeValue &value){
	m_factory.Set (name, value);
}

ApplicationContainer RandomSenderHelper::Install (Ptr<Node> node) const{
	return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer RandomSenderHelper::Install (NodeContainer c) const{
	ApplicationContainer apps;
  	for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i){
      apps.Add (InstallPriv (*i));
    }

  return(apps);
}

Ptr<Application> RandomSenderHelper::InstallPriv (Ptr<Node> node) const{
	NS_LOG_FUNCTION (this << node);

  	Ptr<RandomSender> app = m_factory.Create<RandomSender> ();
  	
	Time initTime;

	initTime = Seconds (m_initialDelay->GetValue ());	
 
  	NS_LOG_DEBUG ("Created an event on timer = " <<
                initTime.GetSeconds() << " Seconds");

 	app->SetInitialDelay (initTime);
  	app->SetPacketSize (m_pktSize);
	app->SetMean (m_initialDelay->GetMean());
  	//app->SetBound (m_initialDelay->GetBound());
  	app->SetNode (node);
  	node->AddApplication (app);

 	return(app);
}

void RandomSenderHelper::SetMean (int mean) {
	m_initialDelay->SetAttribute ("Mean", DoubleValue (mean));
}

void RandomSenderHelper::SetBound (int bound) {
	m_initialDelay->SetAttribute ("Bound", DoubleValue (bound));
}

void RandomSenderHelper::SetPacketSize (uint8_t size){
  	m_pktSize = size;
}

}
} // namespace ns3
