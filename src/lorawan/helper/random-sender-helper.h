/*
 * =====================================================================================
 *
 *       Filename:  random-sender-helper.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  28/01/2019 15:24:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Francisco Helder Candido (FHC), helderhdw@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */


#ifndef RANDOM_SENDER_HELPER_H
#define RANDOM_SENDER_HELPER_H

#include "ns3/object-factory.h"
#include "ns3/address.h"
#include "ns3/attribute.h"
#include "ns3/net-device.h"
#include "ns3/node-container.h"
#include "ns3/application-container.h"
#include "ns3/random-sender.h"
#include <stdint.h>
#include <string>

namespace ns3 {
namespace lorawan{

/**
 * This class can be used to install RandomSender applications on a wide
 * range of nodes.
 */
class RandomSenderHelper{
public:
	RandomSenderHelper ();

  	~RandomSenderHelper ();

  	void SetAttribute (std::string name, const AttributeValue &value);

  	ApplicationContainer Install (NodeContainer c) const;

  	ApplicationContainer Install (Ptr<Node> node) const;
  	/**
   	* Set the period to be used by the applications created by this helper.
   	*
   	* A value of Seconds (0) results in randomly generated periods according to
   	* the model contained in the TR 45.820 document.
   	*
   	* \param period The period to set
   	*/
  	void SetMean (int mean);

 	void SetBound (int bound);

	void SetPacketSize (uint8_t size);

private:
	Ptr<Application> InstallPriv (Ptr<Node> node) const;

  	ObjectFactory m_factory;

  	Ptr<ExponentialRandomVariable> m_initialDelay;

  	uint8_t m_pktSize; // the packet size.
};

}
} // namespace ns3

#endif /* RANDOM_SENDER_HELPER_H */
