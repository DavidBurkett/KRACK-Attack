#include "ConnectionFactory.h"

#include "GUIDUtil.h"
#include "80211hdr.h"

#include <pcap.h>

HANDLE ConnectionFactory::OpenConnection(const Device& device, const Network& network) const
{
	pcap_t* fp;
	char errbuf[PCAP_ERRBUF_SIZE];

	std::string guid = GUIDUtil::GetStringFromGuid(device.guid);
	std::string deviceName = "rpcap://\\Device\\NPF_" + guid;
	/* Open the output device */
	if ((fp = pcap_open(deviceName.c_str(),            // name of the device
		100,                // portion of the packet to capture (only the first 100 bytes)
		PCAP_OPENFLAG_PROMISCUOUS,  // promiscuous mode
		1000,               // read timeout
		NULL,               // authentication on the remote machine
		errbuf              // error buffer
	)) == NULL)
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", guid);
		return NULL;
	}

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
	mgmtHeader.FrameControl.Order = 0;

	mgmtHeader.DurationID = 0x0060;

	memcpy(&mgmtHeader.DA[0], &network.macAddress[0], 6 * sizeof(uint8_t));
	memcpy(&mgmtHeader.SA[0], &device.macAddress[0], 6 * sizeof(uint8_t));
	memcpy(&mgmtHeader.BSSID[0], &network.macAddress[0], 6 * sizeof(uint8_t));
	mgmtHeader.SequenceControl.FragmentNumber = 0x00;
	mgmtHeader.SequenceControl.SequenceNumber = 0x00;


	DOT11_AUTH_FRAME authFrame;
	authFrame.usAlgorithmNumber = DOT11_AUTH_OPEN_SYSTEM;
	authFrame.usXid = 0x01;
	authFrame.usStatusCode = 0x00;

	std::vector<unsigned char> packet;
	packet.insert(packet.begin(), (unsigned char*)&mgmtHeader, (unsigned char*)&mgmtHeader + sizeof(DOT11_MGMT_HEADER));
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



	//char pcap_errbuf[PCAP_ERRBUF_SIZE];
	//pcap_errbuf[0] = '\0';
	//fp = pcap_open_live(deviceName.c_str(), 96, 0, 0, pcap_errbuf);

	//pcap_inject(fp, packet, 100);

	//pcap_set_datalink(fp, DLT_IEEE802_11_RADIO);

	/* Send down the packet */
	if (pcap_sendpacket(fp, &packet[0], packet.size() /* size */) != 0)
	{
		fprintf(stderr, "\nError sending the packet: \n", pcap_geterr(fp));
		return NULL;
	}

	pcap_close(fp);

	return fp;
}