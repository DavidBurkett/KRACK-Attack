#include "ConnectionFactory.h"

#include "GUIDUtil.h"
#include "80211hdr.h"

#include <stdio.h>
#include <stdlib.h>  //for system()
#include <winsock2.h> //for 80211
#include <ntddndis.h>

#define MAC_ADDR_LEN  6
CHAR            PacketuioDevice[] = "\\\\.\\Packet11";
CHAR *          pPacketuioDevice = &PacketuioDevice[0];

#define _NDIS_CONTROL_CODE(request,method) \
            CTL_CODE(FILE_DEVICE_PHYSICAL_NETCARD, request, method, FILE_ANY_ACCESS)

//codes 5, 6, 19 will be supported in a later version
#define IOCTL_PACKET11_SET_OID_VALUE          _NDIS_CONTROL_CODE(5, METHOD_BUFFERED)
#define IOCTL_PACKET11_QUERY_OID_VALUE        _NDIS_CONTROL_CODE(6, METHOD_BUFFERED)
#define IOCTL_PACKET11_INSERT_PACKET		_NDIS_CONTROL_CODE(14, METHOD_BUFFERED)
#define IOCTL_PACKET11_GET_BSSID			_NDIS_CONTROL_CODE(19, METHOD_BUFFERED)
#define IOCTL_PACKET11_GET_MAC				_NDIS_CONTROL_CODE(20, METHOD_BUFFERED)

BOOL IORequest(HANDLE Handle,
	DWORD controlcode,
	PUCHAR input,
	DWORD inputsize,
	PUCHAR output,
	DWORD outputsize
)
{

	DWORD   BytesReturned = 0;
	BOOL status;

	status = (DeviceIoControl(
		Handle,
		controlcode,
		input,
		inputsize,
		output,
		outputsize,
		&BytesReturned,
		NULL));

	if (GetLastError() != 0)
		printf("error %d , bytes returned %d \n", GetLastError(), BytesReturned);

	//Packet11 returns ERROR_NETWORK_ACCESS_DENIED if the user packet could not be allocated
	if (GetLastError() == ERROR_NETWORK_ACCESS_DENIED) printf("Waiting for network resources \n");

	return status;

}

HANDLE ConnectionFactory::OpenConnection(const Device& device, const Network& network) const
{
	std::string guid = GUIDUtil::GetStringFromGuid(device.guid);
	std::string deviceName = "\\\\.\\" + guid;

	std::string deviceNamePacket11 = deviceName + "\\packet11";

	DWORD   DesiredAccess;
	DWORD   ShareMode;
	LPSECURITY_ATTRIBUTES   lpSecurityAttributes = NULL;

	DWORD   CreationDistribution;
	DWORD   FlagsAndAttributes;
	HANDLE  Handle;


	DesiredAccess = GENERIC_READ | GENERIC_WRITE;
	ShareMode = 0;
	CreationDistribution = OPEN_EXISTING;
	FlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

	Handle = CreateFile(
		deviceNamePacket11.c_str()/*deviceName.c_str()*/,
		DesiredAccess,
		ShareMode,
		lpSecurityAttributes,
		CreationDistribution,
		FlagsAndAttributes,
		NULL
	);

	if (Handle == INVALID_HANDLE_VALUE)
	{
		printf("Creating file failed, error %x\n", GetLastError());
		return Handle;
	}

	UCHAR						Source[MAC_ADDR_LEN];
	IORequest(Handle, IOCTL_PACKET11_GET_MAC, NULL, 0, Source, MAC_ADDR_LEN);

	return Handle;
}