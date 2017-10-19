#pragma once

#include "Device.h"

#include <vector>

using namespace std;

// Forward Declarations
struct _WLAN_INTERFACE_INFO;

class DeviceChooser
{
public:
	bool ChooseDevice(Device& device) const;

private:
	bool GetDeviceList(vector<Device>& devices) const;
	Device UnpackDevice(_WLAN_INTERFACE_INFO* pIfInfo) const;

};