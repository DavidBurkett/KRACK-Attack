#pragma once

#include <string>
#include <guiddef.h>

using namespace std;

class GUIDUtil
{
public:
	static string GetStringFromGuid(const GUID& guid);
};