#pragma once

#include "AuthenticationMode.h"
#include "CipherMode.h"

#include <string>
#include <stdint.h>

using namespace std;

struct Network
{
	string profileName;
	string SSID; // TODO: Store the SSID structure, instead.
	uint8_t signalQuality;
	EAuthenticationMode authenticationMode;
	ECipherMode cipherMode;
	bool currentlyConnected;
	bool connectable;
};