# KRACK-Attack
Attempt at demonstrating "Key Reinstallation Attacks: Forcing Nonce Reuse in WPA2"

### NOTE: This is currently in progress, and is expected to be completed ~October 22nd, 2017

## Current Status
#### The following features are implemented so far:
* Displaying and selecting a wireless device (NIC)
* Displaying and selecting a WLAN (by SSID)
* Open the chosen device for communication with the network (using pcap_open)
* Send and receive data packets/frames

#### In Progress:
* Sending 802.11 management frames - WinPCap/npcap doesn't appear to support this, so I'm trying to find a solution. This is the only thing preventing further progress. (https://www.winpcap.org/pipermail/winpcap-users/2005-August/000253.html)


#### Future TODOs:
* Once we are able to send management frames, the rest 'should' be easy. This involves sending and receiving authentication and association requests/responses (association stage), and implementing the 4-way handshake and group key handshakes.

## Compiling
Windows only for now.

1. Install the latest npcap (Do not check the winpcap compatibility checkbox).
2. Download npcap-sdk-0.07-r9.zip from https://github.com/nmap/npcap/releases/tag/v0.07-r9 - Extract it to {KRACK-Attack Directory}\deps\
3. Build KrackAttack.sln (Visual Studio 2017) in the desired configuration (Debug/Release) and Platform (Win32/x64).

I will later provide an older SLN file for those not yet upgraded to VS2017, as well as a CMake file.