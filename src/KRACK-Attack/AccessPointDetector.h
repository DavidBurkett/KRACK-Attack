#pragma once

#include <iostream>
#include <set>
#include <string>
#include <tins/tins.h>

using std::set;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;

using namespace Tins;

class AccessPointDetector {
public:
	void run(const string& iface);
private:
	typedef Dot11::address_type address_type;
	typedef set<address_type> ssids_type;

	bool callback(PDU& pdu);

	ssids_type ssids;
};