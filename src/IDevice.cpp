/***
 * iRecovery++ libusb based usb interface for iBoot and iBSS
 * Copyright (C) 2010  GreySyntax
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "IDevice.h"

#define APPLE_VENDOR_ID 0x05AC

using namespace std;

enum {
	kRecoveryMode	= 0x1281,
	kNormalMode		= 0x1290,
	kWTFMode		= 0x1227,
	kDFUMode		= 0x1222,
	kBufferSize		= 0x10000
};

IDevice::IDevice() {
	
	//Stub
}

bool IDevice::AutoBoot() {
	
	if (! IsConnected) {
		Connect();
	}
}

bool IDevice::Connect() {
	
	if (IsConnected) {
		Close();
	}
	
	if (! USB->Open(APPLE_VENDOR_ID, kRecoveryMode)) {
		
		cout << "[IDevice::Connect] Failed to connect to recovery." << endl;
			
		if (! USB->Open(APPLE_VENDOR_ID, kWTFMode)) {
		
			cout << "[IDevice::Connect] Failed to connect to wtf." << endl;
			
			if (! USB->Open(APPLE_VENDOR_ID, kDFUMode)) {
				
				cout << "[IDevice::Connect] Failed to connect to dfu." << endl;
				return false;
			}
		}
	}
	
	if (! USB->Configure(1)) {
		
		cout << "[IDevice::Connect] Failed to set usb configuration." << endl;
		Close();
		return false;
	}
	
	if (USB->ClaimInterface(0) || USBClaimInterface(1)) {
	
		cout << "[IDevice::Connect] Failed to claim interface's." << endl;
		Close();
		return false;
	}
	
	return true;
}

void IDevice::Disconnect() {
	
	if (! IsConnected) {
		return;
	}
	
	USB->Close();
}

void IDevice::Exploit(const char* file) {
	
	if (! IsConnected) {
		Connect();
	}
	
	
}

bool IDevice::IsConnected() {
	
	if (USB->IsConnected) {
		
		return true;
	}
	
	return false;
}

void IDevice::Reset() {
	
	if (! IsConnected) {
		Connect();
	}
	
	USB->Reset();
}

bool IDevice::SendCommand(const char* argv) {
	
}

bool IDevice::SendBuffer(char* data, int index, int length) {
	
}

bool IDevice::Upload(const char* file) {
	
}