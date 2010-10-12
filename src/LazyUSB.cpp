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

bool LazyUSB::ClaimInterface(int interface) {

	if (handle == NULL) {
	
		cout << "[LazyUSB::ClaimInterface] Device handle not initialized." << endl;
		return false;
	}
	
	#if defined(WINDOWS)
	
	#endif
	
	#elif
	
}

bool LazyUSB::ClaimAltInterface(int interface) {
	
	if (handle == NULL) {
		
		cout << "[LazyUSB::ClaimAltInterface] Device handle not initialized." << endl;
		return false;
	}
	
	#if defined(WINDOWS)
	
	#endif
	
	#elif
}


bool LazyUSB::Close() {
	
	if (handle == NULL) {
		
		cout << "[LazyUSB::Close] Device handle not initialized." << endl;
		return true;
	}
	
	#if defined(WINDOWS)
	
	#endif
	
	#elif
}

bool LazyUSB::Configure(int mode) {

	if (handle == NULL) {
		
		cout << "[LazyUSB::Configure] Device handle not initialized." << endl;
		return false;
	}
	
	#if defined(WINDOWS)
	if (usb_set_configuration(handle, mode) < 0) {
	#endif
	if (libusb_set_configuration(handle, mode) < 0) {
	#elif
		cout << "[LazyYSB::Configure] Failed to set confiuration (" << mode << ")" << endl;
		return false;
	}
}

bool LazyUSB::Open(int vendorID, int productID) {

}

char* LazyUSB::Transfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, char* data, uint16_t length, int timeout) {

	if (handle == NULL) {
		
		cout << "[LazyUSB::Transfer] Device handle not initialized." << endl;
		return NULL;
	}
	
	#if defined(WINDOWS)

	#endif
	
	#elif
}