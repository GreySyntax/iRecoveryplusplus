/***
 * iRecovery++ libusb based usb interface for iBoot and iBSS
 * Copyright © 2010  GreySyntax
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

#ifndef DEVICE_H_
#define DEVICE_H_

#define VENDOR_ID       0x05AC
#define NORM_MODE       0x1290
#define RECV_MODE       0x1281
#define WTF_MODE        0x1227
#define DFU_MODE        0x1222
#define BUF_SIZE        0x10000

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>
#include <readline/history.h>
#include <readline/readline.h>

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}


#define CMD_LOG ".history"

class Device {
public:
	Device();
	bool AutoBoot();
	bool Connect();
	bool Console();
	void Disconnect();
	bool Exploit(const char* file);
	bool IsConnected();
	void Reset();
	bool SendCommand(const char* argv);
	bool SendBuffer(char* data, int index, int length);
	bool Upload(const char* file);

private:
	struct libusb_device_handle *device;

};

#endif /* DEVICE_H_ */
