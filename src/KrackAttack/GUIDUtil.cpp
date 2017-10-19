#include "GUIDUtil.h"

#include <combaseapi.h>

string GUIDUtil::GetStringFromGuid(const GUID& guid)
{
	WCHAR GuidString[39] = { 0 };
	int iRet = StringFromGUID2(guid, (LPOLESTR)&GuidString,
		sizeof(GuidString) / sizeof(*GuidString));

	// For c rather than C++ source code, the above line needs to be
	// iRet = StringFromGUID2(&pIfInfo->InterfaceGuid, (LPOLESTR) &GuidString, 
	//     sizeof(GuidString)/sizeof(*GuidString)); 

	if (iRet != 0)
	{
		wstring wGUID(&GuidString[0], &GuidString[0] + sizeof(GuidString));
		return string(wGUID.cbegin(), wGUID.cend());
	}

	return "";
}