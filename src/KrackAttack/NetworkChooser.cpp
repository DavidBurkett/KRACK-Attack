#ifndef UNICODE
#define UNICODE
#endif

#include "NetworkChooser.h"
#include "GUIDUtil.h"

#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <wtypes.h>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Need to link with Wlanapi.lib and Ole32.lib
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")

bool NetworkChooser::ChooseNetwork(Network& network, const Device& device) const
{
	// Get network list
	vector<Network> networks;
	if (NetworkChooser().GetNetworkList(networks, device))
	{
		printf("NETWORKS:\n");
		for (int i = 0; i < networks.size(); i++)
		{
			printf("Network %d:\n", (i + 1));
			Network& tempNetwork = networks[i];
			printf("\tProfile Name: %s\n", tempNetwork.profileName.c_str());
			printf("\tSSID: %s\n", tempNetwork.SSID.c_str());
			// TODO: Finish this
			printf("\n");
		}

		printf("Enter Network Number (1 - %d): ", (int)networks.size());

		int networkNumber;
		std::cin >> networkNumber;

		while (networkNumber > networks.size())
		{
			printf("Invalid Network Number! Must choose between 1 and %d: ", (int)networks.size());
			std::cin >> networkNumber;
		}

		network = networks[networkNumber - 1];
		return true;
	}
	else
	{
		printf("No Wireless Networks Found!");
		return false;
	}
}

bool NetworkChooser::GetNetworkList(vector<Network>& networks, const Device& device) const
{
	HANDLE hClient = NULL;
	DWORD dwMaxClient = 2;
	DWORD dwCurVersion = 0;
	DWORD dwResult = 0;

	dwResult = WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
	if (dwResult != ERROR_SUCCESS) {
		wprintf(L"WlanOpenHandle failed with error: %u\n", dwResult);
		return false;
	}

	PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
	PWLAN_AVAILABLE_NETWORK pBssEntry = NULL;
	dwResult = WlanGetAvailableNetworkList(hClient, &device.guid, 0, NULL, &pBssList);

	if (dwResult != ERROR_SUCCESS)
	{
		wprintf(L"WlanGetAvailableNetworkList failed with error: %u\n", dwResult);
		return false;
	}

	for (unsigned int j = 0; j < pBssList->dwNumberOfItems; j++) {
		pBssEntry = (WLAN_AVAILABLE_NETWORK *)& pBssList->Network[j];

		networks.push_back(UnpackNetwork(pBssEntry));
	}

	if (pBssList != NULL) {
		WlanFreeMemory(pBssList);
		pBssList = NULL;
	}

	return !networks.empty();
}

Network NetworkChooser::UnpackNetwork(_WLAN_AVAILABLE_NETWORK* pBssEntry) const
{
	Network network;

	wstring wProfileName = pBssEntry->strProfileName;
	network.profileName = string(wProfileName.cbegin(), wProfileName.cend());

	if (pBssEntry->dot11Ssid.uSSIDLength > 0)
	{
		string SSID(&(pBssEntry->dot11Ssid.ucSSID[0]), &(pBssEntry->dot11Ssid.ucSSID[0]) + pBssEntry->dot11Ssid.uSSIDLength);
		network.SSID = SSID;
	}

	network.connectable = pBssEntry->bNetworkConnectable == TRUE;
	network.signalQuality = (uint8_t)pBssEntry->wlanSignalQuality;

	network.authenticationMode = DetermineAuthenticationMode(pBssEntry);
	network.cipherMode = DetermineCipherMode(pBssEntry);
	network.currentlyConnected = (pBssEntry->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED);

	return network;
}

EAuthenticationMode NetworkChooser::DetermineAuthenticationMode(_WLAN_AVAILABLE_NETWORK* pBssEntry) const
{
	switch (pBssEntry->dot11DefaultAuthAlgorithm) 
	{
		case DOT11_AUTH_ALGO_80211_OPEN:
			//wprintf(L"802.11 Open (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
			return EAuthenticationMode::eAuthOpen;
		case DOT11_AUTH_ALGO_80211_SHARED_KEY:
			//wprintf(L"802.11 Shared (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
			return EAuthenticationMode::eAuthWEP; // TODO: Confirm this
		case DOT11_AUTH_ALGO_WPA:
			//wprintf(L"WPA (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
			return EAuthenticationMode::eAuthWPA; // TODO
		case DOT11_AUTH_ALGO_WPA_PSK:
			//wprintf(L"WPA-PSK (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
			return EAuthenticationMode::eAuthWPAPSK;
		case DOT11_AUTH_ALGO_WPA_NONE:
			//wprintf(L"WPA-None (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
			return EAuthenticationMode::eAuthWPA; // TODO
		case DOT11_AUTH_ALGO_RSNA:
			//wprintf(L"RSNA (%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
			break;
		case DOT11_AUTH_ALGO_RSNA_PSK:
			//wprintf(L"RSNA with PSK(%u)\n", pBssEntry->dot11DefaultAuthAlgorithm);
			break;
		default:
			//wprintf(L"Other (%lu)\n", pBssEntry->dot11DefaultAuthAlgorithm);
			break;
	}

	return EAuthenticationMode::eAuthUnknown;
}

ECipherMode NetworkChooser::DetermineCipherMode(_WLAN_AVAILABLE_NETWORK* pBssEntry) const
{
	switch (pBssEntry->dot11DefaultCipherAlgorithm) {
	case DOT11_CIPHER_ALGO_NONE:
		//wprintf(L"None (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
		return ECipherMode::eCipherNone;
	case DOT11_CIPHER_ALGO_WEP40:
		//wprintf(L"WEP-40 (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
		return ECipherMode::eCipherWEP;
	case DOT11_CIPHER_ALGO_TKIP:
		//wprintf(L"TKIP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
		return ECipherMode::eCipherTKIP;
	case DOT11_CIPHER_ALGO_CCMP:
		//wprintf(L"CCMP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
		return ECipherMode::eCipherCCMP;
	case DOT11_CIPHER_ALGO_WEP104:
		//wprintf(L"WEP-104 (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
		return ECipherMode::eCipherWEP;
	case DOT11_CIPHER_ALGO_WEP:
		//wprintf(L"WEP (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
		return ECipherMode::eCipherWEP;
	default:
		//wprintf(L"Other (0x%x)\n", pBssEntry->dot11DefaultCipherAlgorithm);
		break;
	}

	return ECipherMode::eCipherUnknown;
}