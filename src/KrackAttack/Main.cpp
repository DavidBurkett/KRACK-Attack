#include "DeviceChooser.h"
#include "NetworkChooser.h"
#include "ConnectionFactory.h"
#include "80211hdr.h"
#include "AssociationManager.h"

#include <pcap.h>

void main(int argc, char **argv)
{
	//
	// Choose a network interface.
	//
	Device device;
	if (DeviceChooser().ChooseDevice(device))
	{
		printf("\n");

		//
		// Choose a wireless network.
		//
		Network network;
		if (NetworkChooser().ChooseNetwork(network, device))
		{
			printf("Checking if vulnerable....");

			//
			// Establish connection with the wireless network.
			//
			pcap* connection = ConnectionFactory().OpenConnection(device, network);
			if (connection != nullptr)
			{
				if (AssociationManager().AssociateWithNetwork(connection, device, network))
				{
					// TODO: The attack will take place here
				}

				pcap_close(connection);
			}
		}
	}
}