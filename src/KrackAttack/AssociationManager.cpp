#include "AssociationManager.h"
#include "80211hdr.h"
#include "Ether.h"
#include "Ethertype.h"

#include <pcap.h>

#pragma comment(lib, "Ws2_32.lib")

// TODO: Move this to reusable location
struct ieee80211_radiotap_header 
{
	u_int8_t        it_version;     /* set to 0 */
	u_int8_t        it_pad;
	u_int16_t       it_len;         /* entire length */
	u_int32_t       it_present;     /* fields present */
};

//
// The association stage consists of 4 packets:
//
// 1. Authentication Request (from Supplicant to Authenticator).
// 2. Authentication Response (from Authenticator to Supplicant).
// 3. (Re)Association Request (from Supplicant to Authenticator).
// 4. (Re)Association Response (from Authenticator to Supplicant).
//
bool AssociationManager::AssociateWithNetwork(pcap* connection, const Device& device, const Network& network) const
{
	if (SendAuthenticationRequest(connection, device, network))
	{
		if (ReceiveAuthenticationResponse(connection, device, network))
		{
			if (SendAssociationRequest(connection, device, network))
			{
				if (ReceiveAssociationResponse(connection, device, network))
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool AssociationManager::SendAuthenticationRequest(pcap* connection, const Device& device, const Network& network) const
{
	ieee80211_radiotap_header radiotapHeader;
	radiotapHeader.it_version = 0;
	radiotapHeader.it_pad = 0;
	radiotapHeader.it_len = 0x0f;
	radiotapHeader.it_present = 0x2e;

	DOT11_MGMT_HEADER mgmtHeader;
	mgmtHeader.FrameControl.Version = 0;
	mgmtHeader.FrameControl.Type = DOT11_FRAME_TYPE_MANAGEMENT;
	mgmtHeader.FrameControl.Subtype = DOT11_MGMT_SUBTYPE_AUTHENTICATION;
	mgmtHeader.FrameControl.ToDS = 0;
	mgmtHeader.FrameControl.FromDS = 0;
	mgmtHeader.FrameControl.MoreFrag = 0;
	mgmtHeader.FrameControl.Retry = 0;
	mgmtHeader.FrameControl.PwrMgt = 0;
	mgmtHeader.FrameControl.MoreData = 0;
	mgmtHeader.FrameControl.WEP = 0;
	mgmtHeader.FrameControl.Order = 0x08;

	mgmtHeader.DurationID = 0x0060;

	memcpy(&mgmtHeader.DA[0], &network.macAddress[0], 6 * sizeof(uint8_t));
	memcpy(&mgmtHeader.SA[0], &device.macAddress[0], 6 * sizeof(uint8_t));
	memcpy(&mgmtHeader.BSSID[0], &network.macAddress[0], 6 * sizeof(uint8_t));
	mgmtHeader.SequenceControl.FragmentNumber = 0x00;
	mgmtHeader.SequenceControl.SequenceNumber = 0x01;


	DOT11_AUTH_FRAME authFrame;
	authFrame.usAlgorithmNumber = DOT11_AUTH_OPEN_SYSTEM;
	authFrame.usXid = 0x01;
	authFrame.usStatusCode = 0x00;

	std::vector<unsigned char> packet;
	//packet.insert(packet.begin(), (unsigned char*)&radiotapHeader, (unsigned char*)&radiotapHeader + sizeof(ieee80211_radiotap_header));
	packet.insert(packet.end(), (unsigned char*)&mgmtHeader, (unsigned char*)&mgmtHeader + sizeof(DOT11_MGMT_HEADER));
	packet.insert(packet.end(), (unsigned char*)&authFrame, (unsigned char*)&authFrame + sizeof(DOT11_AUTH_FRAME));


	//while (packet.size() <= 40)
	//{
	//	packet.push_back(0);
	//}

	//u_char packet[100];
	//packet[0] = 0xb0;
	//packet[1] = 0x00;
	//packet[2] = 0x00;
	//packet[3] = 0x00;

	//// Router MAC Address
	//packet[4] = network.macAddress[0];
	//packet[5] = network.macAddress[1];
	//packet[6] = network.macAddress[2];
	//packet[7] = network.macAddress[3];
	//packet[8] = network.macAddress[4];
	//packet[9] = network.macAddress[5];

	//// NIC MAC Address
	//packet[10] = device.macAddress[0];
	//packet[11] = device.macAddress[1];
	//packet[12] = device.macAddress[2];
	//packet[13] = device.macAddress[3];
	//packet[14] = device.macAddress[4];
	//packet[15] = device.macAddress[5];

	///* Fill the rest of the packet */
	//for (int i = 12; i < 100; i++)
	//{
	//	packet[i] = i % 256;
	//}

	//pcap_set_datalink(fp, DLT_IEEE802_11_RADIO);

	/* Send down the packet */
	if (pcap_sendpacket(connection, &packet[0], (int)packet.size()) != 0)
	{
		fprintf(stderr, "\nError sending the packet: %s\n", pcap_geterr(connection));
		return false;
	}

	return true;
}

bool AssociationManager::SendAssociationRequest(pcap* connection, const Device& device, const Network& network) const
{
	// TODO: Implement this.
	DOT11_ASSOC_REQUEST_FRAME assocRequest;
	//assocRequest.Capability
	//assocRequest.usListenInterval

	return true;
}

bool AssociationManager::ReceiveAuthenticationResponse(pcap* connection, const Device& device, const Network& network) const
{
	pcap_pkthdr hdr;
	const u_char* packet = pcap_next(connection, &hdr);
	if (packet != nullptr)
	{
		ether_header* ethernetHeader = (ether_header*) packet;

		// TODO: Implement this.
		if (ntohs(ethernetHeader->ether_length_type) == ETHERTYPE_IP)
		{
			printf("Ethernet type hex:%x dec:%d is an IP packet\n",
				ntohs(ethernetHeader->ether_length_type),
				ntohs(ethernetHeader->ether_length_type));
		}
		else  if (ntohs(ethernetHeader->ether_length_type) == ETHERTYPE_ARP)
		{
			printf("Ethernet type hex:%x dec:%d is an ARP packet\n",
				ntohs(ethernetHeader->ether_length_type),
				ntohs(ethernetHeader->ether_length_type));
		}
		else 
		{
			printf("Ethernet type %x not IP", ntohs(ethernetHeader->ether_length_type));
			return false;
		}

		return true;
	}

	return false;
}

bool AssociationManager::ReceiveAssociationResponse(pcap* connection, const Device& device, const Network& network) const
{
	// TODO: Implement this.
	return true;
}