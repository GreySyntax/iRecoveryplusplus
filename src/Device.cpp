//============================================================================
// Name        : Device.cpp
// Author      : GreySyntax
// Version     : 1.0
// Copyright   : GreySyntax 2010(C)
// Description : A C++ implementation of "iRecovery 2.x"
//============================================================================

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "headers/Device.h"

using namespace std;

Device::Device() {

	libusb_init(NULL);
}

void Device::AutoBoot() {

	cout << "[Device] Enabling auto-boot." << endl;

	if(Send("setenv auto-boot true")) {
		if (Send("saveenv")) {
			if (Send("reboot")) {

				Disconnect();
				cout << "[Device] Enabled auto-boot." << endl;
				exit(1);
			}
		}
	}

	cout << "[Device] Failed to enable auto-boot." << endl;
	exit(-1);
}

bool Device::Buffer(char* data, int length) {

	//TODO
}

bool Device::Connect() {

	if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, RECV_MODE)) == NULL) {
		if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, WTF_MODE)) == NULL) {
			if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, DFU_MODE)) == NULL) {
				return false;
			}
		}
	}

	int configuration = 0;
	libusb_get_configuration(device, &configuration);

	if (configuration != 1) {

		if(libusb_set_configuration(device, 1) < 0) {

			cout << "[Device] Failed to set configuration." << endl;
			return false;
		}
	}

	if (libusb_claim_interface(device, 0) < 0) {
		cout << "[Device] Error claiming interface." << endl;
		//If at first you don't succeed, try, try again.
		//Then quit. There's no point in being a damn fool about it.
		return false;
	}

	//It's not enough that I should succeed - others should fail.
	return true;
}

void Device::Disconnect() {

	if (device != NULL) {
		cout << "[Device] Closing USB Connection." << endl;
		libusb_release_interface(device, 0);
		libusb_release_interface(device, 1);
		libusb_close(device);
	}
}

void Device::Reset() {

	libusb_reset_device(device);
}

bool Device::Send(const char* command) {

	int length = strlen(command);

	if (length >= 0x100) {
		cout << "[Device] Failed to send command (to long)." << endl;
		return false;
	}

	if(! libusb_control_transfer(device, 0x40, 0, 0, 0, (unsigned char*)command, (length + 1), 500)) {
		cout << "[Device] Failed to send command" << endl;
		return false;
	}
	return true;
}

bool Device::Upload(const char* file) {

	FILE* data = fopen(file, "rb");

	if (data != NULL) {

		fseek(data, 0, SEEK_END);
		unsigned int length = ftell(data);
		fseek(data, 0, SEEK_SET);

		char* buffer = (char*)malloc(length);

		if (buffer != NULL) {

			fread(buffer, 1, length, data);
			fclose(data);

			int packets, last;

			packets = length % 0x800;
			last = length % 0x800;

			if (length % 0x800) packets++;
			if (! last) last = 0x800;

			unsigned int sent;
			char response[6];

			for (int i = 0; i < packets; i++) {

				int len = last;

				if ((i + 1) < packets) len = 0x800;
				sent += len;

				if (libusb_control_transfer(device, 0x21, 1, i, 0, (unsigned char*)&buffer[i * 0x800], len, 1000)) {

					if (libusb_control_transfer(device, 0xA1, 3, 0, 0, (unsigned char*)response, 6, 1000) == 6) {

						if (response[4] == 5) {

							cout << "[IO] Sucessfully transfered " << (i + 1) << " of " << packets << endl;
							continue;
						}

						cout << "[Device] Invalid execution status" << endl;
						free(buffer);
						return false;
					}

					cout << "[Device] Failed to retreive execution status" << endl;
					free(buffer);
					return false;
				}

				cout << "[IO] Failed to send packet " << (i + 1) << " of " << packets << endl;
				free(buffer);
				return false;
			}

			libusb_control_transfer(device, 0x21, 1, packets, 0, (unsigned char*)buffer, 0, 1000);
			free(buffer);
			cout << "[Device] Executing file" << endl;

			unsigned char test[6];
			for (int i = 6; i <= 8; i++) {

				if (libusb_control_transfer(device, 0xA1, 3, 0, 0, test, 6, 1000) == 6) {
					if (test[4] != i) {
						cout << (char*)test << endl;
						cout << "[Device] Invalid execution status.." << endl;
						return false;
					}
					continue;
				}
				return false;
			}

			cout << "[Device] Successfully executed file" << endl;
			return true;
		}

		cout << "[IO] Failed to allocate " << length << " bytes" << endl;
		fclose(data);
		return false;
	}

	cout << "[IO] Failed to open file " << file << endl;
	return false;
}
