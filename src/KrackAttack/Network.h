#pragma once

#include "AuthenticationMode.h"
#include "CipherMode.h"

#include <string>
#include <stdint.h>
#include <vector>

using namespace std;

struct Network
{
	string profileName;
	string SSID; // TODO: Store the SSID structure, instead.
	vector<unsigned char> macAddress;
	uint8_t signalQuality;
	EAuthenticationMode authenticationMode;
	ECipherMode cipherMode;
	bool currentlyConnected;
	bool connectable;
};