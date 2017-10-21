#pragma once

#include <WinSock2.h>

#include "Device.h"
#include "Network.h"

// Forward Declarations
struct pcap;

class ConnectionFactory
{
public:
	pcap* OpenConnection(const Device& device, const Network& network) const;
};