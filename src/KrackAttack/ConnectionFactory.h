#pragma once

#include <winsock2.h>

#include "Device.h"
#include "Network.h"

class ConnectionFactory
{
public:
	HANDLE OpenConnection(const Device& device, const Network& network) const;

private:
	unsigned char* GetPacket() const;
};