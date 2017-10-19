#pragma once

#include "Network.h"
#include "Device.h"

#include <vector>

using namespace std;

// Forward declarations
struct _WLAN_AVAILABLE_NETWORK;

class NetworkChooser
{
public:
	bool ChooseNetwork(Network& network, const Device& device) const;

private:
	bool GetNetworkList(vector<Network>& networks, const Device& device) const;
	Network UnpackNetwork(_WLAN_AVAILABLE_NETWORK* pBssEntry) const;
	EAuthenticationMode DetermineAuthenticationMode(_WLAN_AVAILABLE_NETWORK* pBssEntry) const;
	ECipherMode DetermineCipherMode(_WLAN_AVAILABLE_NETWORK* pBssEntry) const;
};