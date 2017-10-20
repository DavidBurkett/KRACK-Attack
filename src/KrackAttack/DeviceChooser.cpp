#ifndef UNICODE
#define UNICODE
#endif

#include "DeviceChooser.h"
#include "GUIDUtil.h"

#include <iostream>
#include <windows.h>
#include <wlanapi.h>

#include <Iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")

bool DeviceChooser::ChooseDevice(Device& device) const
{
	// Get device list.
	vector<Device> devices;
	if (GetDeviceList(devices))
	{
		printf("DEVICES:\n");
		for (int i = 0; i < devices.size(); i++)
		{
			printf("Device %d:\n", (i + 1));
			Device& tempDevice = devices[i];
			printf("\tGUID: %s\n", GUIDUtil::GetStringFromGuid(tempDevice.guid).c_str());
			printf("\tDescription: %s\n", tempDevice.description.c_str());
			printf("\n");
		}

		printf("Enter Device Number (1 - %d): ", (int)devices.size());

		int deviceNumber;
		std::cin >> deviceNumber;

		while (deviceNumber > devices.size())
		{
			printf("Invalid Device Number! Must choose between 1 and %d: ", (int)devices.size());
			std::cin >> deviceNumber;
		}

		device = devices[deviceNumber - 1];
		return true;
	}
	else
	{
		printf("No Network Devices Found!");
		return false;
	}
}

bool DeviceChooser::GetDeviceList(vector<Device>& devices) const
{
	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;  
	DWORD dwCurVersion = 0;

	DWORD dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return false;
		// You can use FormatMessage here to find out why the function failed
	}

	PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
	dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList);
	if (dwResult != ERROR_SUCCESS)
	{
		wprintf(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
		return false;
		// You can use FormatMessage here to find out why the function failed
	}

	if (pIfList != NULL) 
	{
		devices.resize(pIfList->dwNumberOfItems);
		wprintf(L"Num Entries: %lu\n", pIfList->dwNumberOfItems);
		wprintf(L"Current Index: %lu\n", pIfList->dwIndex);
		for (int i = 0; i < (int)pIfList->dwNumberOfItems; i++)
		{
			PWLAN_INTERFACE_INFO pIfInfo = (WLAN_INTERFACE_INFO *)&pIfList->InterfaceInfo[i];

			devices[i] = UnpackDevice(pIfInfo);
		}

		WlanFreeMemory(pIfList);
		pIfList = NULL;
	}


	IP_ADAPTER_INFO *info = NULL, *pos;
	DWORD size = 0;
	GetAdaptersInfo(info, &size);

	info = (IP_ADAPTER_INFO *)malloc(size);

	GetAdaptersInfo(info, &size);

	for (pos = info; pos != NULL; pos = pos->Next) 
	{
		for (Device& device : devices)
		{
			std::string guid = GUIDUtil::GetStringFromGuid(device.guid);
			std::string adapterName = std::string(pos->AdapterName);
			if (guid == adapterName)
			{
				for (int i = 0; i < pos->AddressLength; i++)
					device.macAddress.push_back(pos->Address[i]);
			}
		}
	}

	free(info);

	return !devices.empty();
}

Device DeviceChooser::UnpackDevice(_WLAN_INTERFACE_INFO* pIfInfo) const
{
	Device device;
	device.guid = pIfInfo->InterfaceGuid;

	wstring wDescription = pIfInfo->strInterfaceDescription;
	device.description = string(wDescription.cbegin(), wDescription.cend());

	return device;
}