# KRACK-Attack
Attempt at demonstrating "Key Reinstallation Attacks: Forcing Nonce Reuse in WPA2"

### NOTE: This is currently in progress, and is expected to be completed October 20th, 2017

## Compiling
Windows only for now.

1. Install the latest npcap (Do not check the winpcap compatibility checkbox).
2. Download npcap-sdk-0.07-r9.zip from https://github.com/nmap/npcap/releases/tag/v0.07-r9 - Extract it to {KRACK-Attack Directory}\deps\
3. Build KrackAttack.sln (Visual Studio 2017) in the desired configuration (Debug/Release) and Platform (Win32/x64).

I will later provide an older SLN file for those not yet upgraded to VS2017, as well as a CMake file.