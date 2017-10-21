#include "ConnectionFactory.h"

#include "GUIDUtil.h"
#include "80211hdr.h"

#include <WinSock2.h>
#include <pcap.h>

pcap* ConnectionFactory::OpenConnection(const Device& device, const Network& network) const
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
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n", guid.c_str());
		return NULL;
	}

	return fp;
}