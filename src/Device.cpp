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

	libusb_control_transfer(device, 0x40, 0, 0, 0, (unsigned char*)command, (length + 1), 1000);
	return true;
}
