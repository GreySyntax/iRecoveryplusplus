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

#ifndef IDEVICE_H_

#include "LazyUSB.h"
#include "Common.h"

#define IDEVICE_H_
#define APPLE_VENDOR_ID		0x05AC
#define LOG_FILE			".irecovery_history"

class IDevice {
	
public:
	
	//Constructors
	IDevice();
	
	//Methods
	bool AutoBoot();
	bool Connect();
	bool Console();
	void Disconnect();
	bool Exploit(const char* file);
	bool IsConnected();
	void Reset();
	bool SendCommand(const char* argv);
	bool SendBuffer(char* data, int length, int* actual_length);
    void Shell();
	bool Upload(const char* file);
	
private:
	
	//Properties
	LazyUSB USB;
};

#endif /* IDEVICE_H_ */
