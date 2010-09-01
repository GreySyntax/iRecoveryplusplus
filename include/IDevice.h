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

#ifndef IDEVICE_H_

#define IDEVICE_H_
#define APPLE_VENDOR_ID 0x05AC

enum {
	kRecoveryMode	= 0x1281,
	kNormalMode		= 0x1290,
	kWTFMode		= 0x1227,
	kDFUMode		= 0x1222,
	kBufferSize		= 0x10000
};

class USBDevice {

public:
	virtual bool AutoBoot();
	virtual bool Connect();
	virtual bool Console();
	virtual void Disconnect();
	virtual bool Exploit(const char* file);
	virtual bool IsConnected();
	virtual void Reset();
	virtual bool SendCommand(const char* argv);
	virtual bool SendBuffer(char* data, int index, int length);
	virtual bool Upload(const char* file);

private:
#if defined(WINDOWS)
	struct usb_device_handle *handle;
#else
	struct libusb_device_handle *handle;
#endif
};

#endif /* IDEVICE_H_ */
