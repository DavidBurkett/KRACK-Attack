#include "DeviceChooser.h"
#include "NetworkChooser.h"
#include "ConnectionFactory.h"
#include "80211hdr.h"

#include <iostream>

void main(int argc, char **argv)
{
	// Get Device
	Device device;
	if (DeviceChooser().ChooseDevice(device))
	{
		printf("\n");
		Network network;
		if (NetworkChooser().ChooseNetwork(network, device))
		{
			printf("Checking if vulnerable....");
			HANDLE deviceHandle = ConnectionFactory().OpenConnection(device, network);
			if (deviceHandle != INVALID_HANDLE_VALUE)
			{
				// TODO: The attack will take place here
				//CloseHandle(deviceHandle);
			}
		}
	}
}