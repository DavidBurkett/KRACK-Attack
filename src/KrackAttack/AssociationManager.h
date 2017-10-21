#pragma once

#include <WinSock2.h>
#include "Device.h"
#include "Network.h"

// Forward Declarations
struct pcap;

//
// Associaties with a network by managing the sending and receiving of 802.11 authentication and (re)association packets.
//
class AssociationManager
{
public:
	//
	// Associates the given device with the given network.
	// Returns true if the association stage is successful. Otherwise, false.
	//
	bool AssociateWithNetwork(pcap* connection, const Device& device, const Network& network) const;

private:
	bool SendAuthenticationRequest(pcap* connection, const Device& device, const Network& network) const;
	bool SendAssociationRequest(pcap* connection, const Device& device, const Network& network) const;

	bool ReceiveAuthenticationResponse(pcap* connection, const Device& device, const Network& network) const;
	bool ReceiveAssociationResponse(pcap* connection, const Device& device, const Network& network) const;
};