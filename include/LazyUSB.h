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

#include "Win32.h"
#if defined(WINDOWS)
	/* lets support this at the lowest level! */
	#include <usb.h>
#else
	#include <libusb-1.0/libusb.h>
#endif

#ifndef LAZYUSB_H_

#define LAZYUSB_H_

class LazyUSB {

public:

	bool ClaimInterface(int interface);
	bool ClaimAltInterface(int interface);
	bool Close();
	bool Configure(int mode);

	bool Open(int vendorID, int productID);

	bool ReleaseInterface(int interface);
	void Reset();

	int Transfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, char* data, uint16_t length, int timeout);

private:
	#if defined(WINDOWS)
	struct usb_device_handle *handle;
	#else
	struct libusb_device_handle *handle;
	#endif
};
#endif /* LAZYUSB_H_ */
