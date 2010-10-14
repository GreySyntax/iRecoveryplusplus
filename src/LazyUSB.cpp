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

#include "LazyUSB.h"

using namespace std;

enum {
	kRecoveryMode	= 0x1281,
	kNormalMode		= 0x1290,
	kWTFMode		= 0x1227,
	kDFUMode		= 0x1222,
	kBufferSize		= 0x10000
};

LazyUSB::LazyUSB() {
		
	handle = NULL;
}

bool LazyUSB::ClaimInterface(int interface) {

	if (handle == NULL) {
	
		cout << "[LazyUSB::ClaimInterface] Device handle not initialized." << endl;
		return false;
	}
	
	#if defined(WINDOWS)
	if (usb_claim_interface(handle, interface) < 0) {
	#else
	if (libusb_claim_interface(handle, interface) < 0) {
	#endif
		cout << "[LazyUSB::ClaimInterface] Failed to claim usb interface (" << interface << ")." << endl;
		return false;
	}
		
	return true;
	
}

bool LazyUSB::ClaimAltInterface(int interface) {
	
	if (handle == NULL) {
		
		cout << "[LazyUSB::ClaimAltInterface] Device handle not initialized." << endl;
		return false;
	}
	
	#if defined(WINDOWS)
	if (usb_set_altinterface(handle, interface) < 0) {
		cout << "[LazyUSB::ClaimAltInterface] Failed to claim alt interface (" << interface << ")." << endl;
		return false;
	}
	
	return true;
	#else
	//Not supported directly -- AVOID USING IF POSSIBLE
	return ClaimInterface(interface);
	#endif
}


bool LazyUSB::Close() {
	
	if (handle == NULL) {
		
		cout << "[LazyUSB::Close] Device handle not initialized." << endl;
		return false;
	}
	
	#if defined(WINDOWS)
	usb_close(handle);
	#else
	libusb_close(handle);
	#endif
	
	handle = NULL;
	return true;
}

bool LazyUSB::Configure(int mode) {

	if (handle == NULL) {
		
		cout << "[LazyUSB::Configure] Device handle not initialized." << endl;
		return false;
	}
	
	#if defined(WINDOWS)
	if (usb_set_configuration(handle, mode) < 0) {
	#else
	if (libusb_set_configuration(handle, mode) < 0) {
	#endif
		cout << "[LazyUSB::Configure] Failed to set confiuration (" << mode << ")" << endl;
		return false;
	}
	
	return true;
}

bool LazyUSB::Open(int vendorID, int productID) {


	handle = NULL;
	
	#if defined(WINDOWS)
	
	struct usb_device *dev = NULL;
	struct usb_bus *bus = NULL;
	
	usb_init();
	usb_find_busses();
	usb_find_devices();
	
	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			if (dev->descriptor.idVendor == vendorID && dev->descriptor.idProduct == productID) {
				handle = usb_open(dev);
			}
		}
	}

	if (handle == NULL) {	
	#else
	libusb_init(NULL);
	
	if ((handle = libusb_open_device_with_vid_pid(NULL, vendorID, productID)) == NULL) {
	#endif
		cout << "[LazyUSB::Open] Failed to open usb device (Vendor: " << vendorID << ", ProductID: " << productID << ")" << endl;
		return false;
	}
		
	cout << "[LazyUSB::Open] Opened device with VendorID: " << vendorID << " and ProductID: " << productID << "." << endl;
		
	return true;
}

bool LazyUSB::ReleaseInterface(int interface) {
	
	if (handle == NULL) {
		
		cout << "[LazyUSB::ReleaseInterface] Device handle not initialized." << endl;
		return false;
	}
	
	#if defined(WINDOWS)
	usb_release_interface(handle, interface);
	#else
	libusb_release_interface(handle, interface);
	#endif
	
	return true;
}

void LazyUSB::Reset() {
	
	
	if (handle == NULL) {
		
		cout << "[LazyUSB::Reset] Device handle not initialized." << endl;
		return;
	}
	
	#if defined(WINDOWS)
	usb_reset(handle);
	#else
	libusb_reset_device(handle);
	#endif	
}
	
int LazyUSB::Transfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, const char* data, uint16_t length, int timeout) {

	if (handle == NULL) {
		
		cout << "[LazyUSB::Transfer] Device handle not initialized." << endl;
		return NULL;
	}
	
	int res = 0;
	
	#if defined(WINDOWS)
	res = usb_control_msg(handle, requestType, request, value, index, &data, length, timeout);
	#else
	res = libusb_control_transfer(handle, requestType, request, value, index, (unsigned char*)&data, length, timeout);
	#endif
	
	return res;
}
	
int LazyUSB::Write(int endPoint, char *data, int length, int* actual_length, int timeout) {
		
	if (handle == NULL) {
	
		cout << "[LazyUSB::Write] Device handle not initialized." << endl;
		return -1;
	}
	
	int res = 0;
	
	#if defined(WINDOWS)
	res = usb_bulk_write(handle, endPoint, data, length, timeout);
	#else
	res = libusb_bulk_transfer(handle, (unsigned char)endPoint, (unsigned char*)data, length, actual_length, timeout);
	#endif
	
	return res;
}

bool LazyUSB::IsConnected() {

	if (handle == NULL) {
		
		return false;
	}
	
	return true;
}