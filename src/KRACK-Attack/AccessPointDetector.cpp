#include "stdafx.h"

#include "AccessPointDetector.h"

void AccessPointDetector::run(const std::string& iface) {
	SnifferConfiguration config;
	config.set_promisc_mode(true);
	config.set_filter("type mgt subtype beacon");
	config.set_rfmon(true);
	Sniffer sniffer(iface, config);
	sniffer.sniff_loop(make_sniffer_handler(this, &AccessPointDetector::callback));
}

bool AccessPointDetector::callback(PDU& pdu) {
	// Get the Dot11 layer
	const Dot11Beacon& beacon = pdu.rfind_pdu<Dot11Beacon>();
	// All beacons must have from_ds == to_ds == 0
	if (!beacon.from_ds() && !beacon.to_ds()) {
		// Get the AP address
		address_type addr = beacon.addr2();
		// Look it up in our set
		ssids_type::iterator it = ssids.find(addr);
		if (it == ssids.end()) {
			// First time we encounter this BSSID.
			try {
				/* If no ssid option is set, then Dot11::ssid will throw
				* a std::runtime_error.
				*/
				string ssid = beacon.ssid();
				// Save it so we don't show it again.
				ssids.insert(addr);
				// Display the tuple "address - ssid".
				cout << addr << " - " << ssid << endl;
			}
			catch (runtime_error&) {
				// No ssid, just ignore it.
			}
		}
	}
	return true;
}