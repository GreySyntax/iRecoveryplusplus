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
	kBufferSize		= 0x10000,
	kCommandMaxLen	= 0x200,
	kCommandTimeout = 500,
	kUploadTimeout	= 1000
};

IDevice::IDevice() {
	
	//Stub
}

bool IDevice::AutoBoot() {
	
	if (! USB.IsConnected()) {
		Connect();
	}
}

bool IDevice::Connect() {
	
	if (USB.IsConnected()) {
		Disconnect();
	}
	
	if (! USB.Open(APPLE_VENDOR_ID, kRecoveryMode)) {
		
		cout << "[IDevice::Connect] Failed to connect to recovery." << endl;
			
		if (! USB.Open(APPLE_VENDOR_ID, kWTFMode)) {
		
			cout << "[IDevice::Connect] Failed to connect to wtf." << endl;
			
			if (! USB.Open(APPLE_VENDOR_ID, kDFUMode)) {
				
				cout << "[IDevice::Connect] Failed to connect to dfu." << endl;
				return false;
			}
		}
	}
	
	if (! USB.Configure(1)) {
		
		cout << "[IDevice::Connect] Failed to set usb configuration." << endl;
		Disconnect();
		return false;
	}
	
	if (! USB.ClaimInterface(0) && ! USB.ClaimInterface(1)) {
	
		cout << "[IDevice::Connect] Failed to claim interface's." << endl;
		//Disconnect();
		return false;
	}
	
	return true;
}

void IDevice::Disconnect() {
	
	if (! USB.IsConnected()) {
		return;
	}
	
	USB.Close();
	cout << "[IDevice::Disconnect] Closed connection." << endl;
}

bool IDevice::Exploit(const char* file) {
	
	if (! USB.IsConnected()) {
		Connect();
	}
	
	
}

void IDevice::Reset() {
	
	if (! USB.IsConnected()) {
		Connect();
	}
	
	USB.Reset();
}

bool IDevice::SendCommand(const char* argv) {
	
	int length = strlen(argv);
	
	if (length > kCommandMaxLen) {
	
		cout << "[IDevice::SendCommand] Command to long, aborting." << endl;
		return false;
	}
	
	if (! USB.IsConnected()) {
		Connect();
	}
	
	if (! USB.Transfer(0x40, 0, 0, 0, argv, (length + 1), kCommandTimeout)) {
	
		cout << "[IDevice::SendCommand] Failed to send command." << endl;
		return false;
	}
	
	char* action = strtok(strdup(argv), " ");
	
	if (! strcmp(action, "getenv")) {
		
		char response[0x200];
		USB.Transfer(0xC0, 0, 0, 0, response, 0x200, (kCommandTimeout * 2));
		cout << response << endl;
	}
	
	return true;
}

bool IDevice::SendBuffer(char* data, int length, int* actual_length) {
	
	if (! USB.Write(4, data, length, actual_length, kUploadTimeout)) {
		return false;
	}
	
	return true;
}

bool IDevice::Upload(const char* file) {
	
	FILE* data = fopen(file, "rb");

	if (data == NULL) {
		cout << "[IDevice::Upload] Failed to open file " << file << endl;
		return false;
	}
	
	cout << "[IDevice::Upload] Attemtping to upload file" << endl;

	fseek(data, 0, SEEK_END);
	unsigned int length = ftell(data);
	fseek(data, 0, SEEK_SET);

	char* buffer = (char*)malloc(length);

	if (buffer == NULL) {

		cout << "[IDevice::Upload] Failed to allocate " << length << " bytes" << endl;
		fclose(data);
		return false;
	}

	fread(buffer, 1, length, data);
	fclose(data);

	int actual_length = (int)length;
	
	return SendBuffer(buffer, length, &actual_length);
}
