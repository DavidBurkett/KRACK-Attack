#pragma once

#include <string>
#include <guiddef.h>
#include <vector>

using namespace std;

struct Device
{
	GUID guid;
	string description;
	vector<unsigned char> macAddress;
};