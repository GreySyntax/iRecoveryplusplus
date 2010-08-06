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

#include "headers/USBDevice.h"
#include "headers/Program.h"

using namespace std;

/***
 * Initialize the usb api.
 */
USBDevice::USBDevice() {

	//stub
}

/***
 * Set the auto-boot environment variable to true,
 * this can be used to exit some recovery loops.
 */
bool USBDevice::AutoBoot() {

	if(SendCommand("setenv auto-boot true")) {
		if (SendCommand("saveenv")) {
			if (SendCommand("reboot")) {

				cout << "[Info] Enabled auto-boot." << endl;
				return true;
			}
		}
	}

	cout << "[Info] Failed to enable auto-boot." << endl;
	return false;
}

/***
 * Connect to the device, this will loop through recovery, wtf and dfu mode while searching.
 * This also claim's the USB interface so make sure to free them propperly!
 */
bool USBDevice::Connect() {

	if (device != NULL)
		Disconnect();

	libusb_init(NULL);

	if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, RECV_MODE)) == NULL) {
		if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, WTF_MODE)) == NULL) {
			if ((device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, DFU_MODE)) == NULL) {
				cout << "[Info] Failed to connect." << endl;
				return false;
			}
		}
	}

	int configuration = 0;
	libusb_get_configuration(device, &configuration);

	if (configuration != 1) {

		if(libusb_set_configuration(device, 1) < 0) {

			cout << "[Info] Failed to set configuration." << endl;
			return false;
		}
	}

	if (libusb_claim_interface(device, 0) < 0 || libusb_claim_interface(device, 1) < 0) {
		cout << "[Info] Error claiming interface." << endl;
		//If at first you don't succeed, try, try again.
		//Then quit. There's no point in being a damn fool about it.
		return false;
	}

	//It's not enough that I should succeed - others should fail.
	return true;
}


/***
 * Initialize a terminal session with iBoot/iBSS.
 * This is a command promt like interface (output from iBoot is shit).
 */
bool USBDevice::Console() {

	if (libusb_set_interface_alt_setting(device, 1, 1) < 0)
	{
		cout << "[Info] Error setting interface." << endl;
		return false;
	}

	char* buffer;
	if ((buffer = (char*)malloc(BUF_SIZE)) == NULL) {
		cout << "[Info] Failed to allocate memory." << endl;
		return false;
	}

	read_history(CMD_LOG);
	int bytes = 0, i = 0;

	while (true) {

		bytes = 0;
		memset(buffer, 0 , BUF_SIZE);

		libusb_bulk_transfer(device, 0x81, (unsigned char*)buffer, BUF_SIZE, &bytes, 500);

		if (bytes > 0) {
			for (i = 0; i < bytes; i++) {
				cout << (char)buffer[i];
			}
			cout << endl;
		}

		//Grab input
		char* command = readline("MobileDevice$ ");

		if (command != NULL) {
			add_history(command);
			write_history(CMD_LOG);

			if (command[0] != '/')
				SendCommand(command);
			else
				call_plugin(string(command).substr(1).c_str());
		}
	}

	return true;
}

/***
 * Close the connection and free the interfaces.
 */
void USBDevice::Disconnect() {

	if (device != NULL) {
		cout << "\r\n[Info] Closing USB Connection." << endl;
		libusb_release_interface(device, 0);
		libusb_release_interface(device, 1);
		libusb_close(device);
		device = NULL;
	}
}

/***
 * Upload a file (if specified) then attempt to use the usb control exploit.
 */
bool USBDevice::Exploit(const char* file) {

	cout << "[Info] Attempting to send exploit" << endl;

	if (file != NULL)
		Upload(file);

	if (! libusb_control_transfer(device, 0x21, 2, 0, 0, 0, 0, 1000)) {

		cout << "[Info] Failed to send exploit" << endl;
		return false;
	}

	cout << "[Info] Succesfully sent exploit" << endl;
	return true;
}

/***
 * Check if were connected to a device.
 */
bool USBDevice::IsConnected() {

	return device == NULL ? false : true;
}

/***
 * Reset the usb connection/interfaces saves unplugging and replugging the device.
 */
void USBDevice::Reset() {

	libusb_reset_device(device);
}

/***
 * Send a char buffer to the device,
 * we can send from any position in the buffer aslong as the remaning data is long enough.
 */
bool USBDevice::SendBuffer(char* buffer, int index, int length) {

	int packets = length / 0x800;

	if(length % 0x800)
		packets++;

	int last = length % 0x800;

	if(!last)
		last = 0x800;

	int i = 0;
	unsigned int sizesent=0;

	char response[6];

	for(i = 0; i < packets; i++) {

		int size = i + 1 < packets ? 0x800 : last;

		sizesent+=size;
		printf("[Device] Sending packet %d of %d (0x%08x of 0x%08x bytes)\r\n", i+1, packets, sizesent, length);

		if(! libusb_control_transfer(device, 0x21, 1, i, 0, (unsigned char*)&buffer[i * 0x800], size, 1000)) {

			printf("[Device] Error sending packet.\r\n");
			return -1;

		}

		if( libusb_control_transfer(device, 0xA1, 3, 0, 0, (unsigned char*)response, 6, 1000) != 6) {

			printf("[Device] Error receiving status while uploading file.\r\n");
			return -1;

		}

		if(response[4] != 5) {

			printf("[Device] Invalid status error during file upload.\r\n");
			return -1;
		}

		printf("[Device] Upload successfull.\r\n");
	}

	printf("[Device] Executing file.\r\n");

	libusb_control_transfer(device, 0x21, 1, i, 0, (unsigned char*)buffer, 0, 1000);

	for(i = 6; i <= 8; i++) {

		if(libusb_control_transfer(device, 0xA1, 3, 0, 0, (unsigned char*)response, 6, 1000) != 6) {

			printf("[Device] Error receiving execution status.\r\n");
			return -1;
		}

		if(response[4] != i) {

			printf("[Device] Invalid execution status.\r\n");
			return -1;
		}
	}

	/*
	 for (int i = 0; i < packets; i++) {

		int len = last;

		if ((i + 1) < packets)
			len = 0x800;

		sent += len;

		if (libusb_control_transfer(device, 0x21, 1, i, 0, (unsigned char*)&buffer[index + (i * 0x800)], len, 1000)) {
			if (libusb_control_transfer(device, 0xA1, 3, 0, 0, (unsigned char*)response, 6, 1000) == 6) {
				if (response[4] == 5) {

					//cout << "[IO] Sucessfully transfered " << (i + 1) << " of " << packets << endl;
					continue;
				}

				cout << "[Info] Invalid execution status" << endl;
				free(buffer);
				return false;
			}

			cout << "[Info] Failed to retreive execution status" << endl;
			free(buffer);
			return false;
		}

		cout << "[IO] Failed to send packet " << (i + 1) << " of " << packets << endl;
		free(buffer);
		return false;
	}

	libusb_control_transfer(device, 0x21, 1, packets, 0, (unsigned char*)buffer, 0, 1000);
	free(buffer);
	cout << "[Info] Executing" << endl;

	for (int i = 6; i <= 8; i++) {

		if (libusb_control_transfer(device, 0xA1, 3, 0, 0, (unsigned char*)response, 6, 1000) == 6) {
			if (response[4] != i) {

				cout << "[Info] Invalid status recived from device" << endl;
				return false;
			}
			continue;
		}
		return false;
	}*/

	cout << "[Info] Successfully transfered buffer" << endl;
	return true;
}

/***
 * Send a standard command to iBoot/iBSS.
 */
bool USBDevice::SendCommand(const char* command) {

	int length = strlen(command);

	if (length >= 0x200) {
		cout << "[Info] Failed to send command (to long)." << endl;
		return false;
	}

	if(! libusb_control_transfer(device, 0x40, 0, 0, 0, (unsigned char*)command, (length + 1), 500)) {
		cout << "[Info] Failed to send command" << endl;
		return false;
	}

	char* action = strtok(strdup(command), " ");

	if (! strcmp(action, "getenv")) {
		unsigned char response[0x200];
		libusb_control_transfer(device, 0xC0, 0, 0, 0, response, 0x200, 1000);
		cout << response << endl;
	}

	cout << "[Info] Sent " << command << " to device" << endl;
	return true;
}

/***
 * Read the contents of a file to a char array and call SendBuffer.
 */
bool USBDevice::Upload(const char* file) {

	FILE* data = fopen(file, "rb");

	if (data != NULL) {

		cout << "[Info] Attemtping to upload file" << endl;

		fseek(data, 0, SEEK_END);
		unsigned int length = ftell(data);
		fseek(data, 0, SEEK_SET);

		char* buffer = (char*)malloc(length);

		if (buffer != NULL) {

			fread(buffer, 1, length, data);
			fclose(data);

			return SendBuffer(buffer, 0, length);

		}

		cout << "[IO] Failed to allocate " << length << " bytes" << endl;
		fclose(data);
		return false;
	}

	cout << "[IO] Failed to open file " << file << endl;
	return false;
}
