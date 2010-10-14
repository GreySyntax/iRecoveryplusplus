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
 
#include <iostream>
#include "IDevice.h"

using namespace std;

static IDevice Device;

int main(int argc, char *argv[]) {

	cout << "iRecovery++  Copyright (C) 2010  GreySyntax\r\n";
	cout << "This program comes with ABSOLUTELY NO WARRANTY; for details `./iRecovery -w'.\r\n";
	cout << "This is free software, and you are welcome to redistribute it\r\n";
	cout << "under certain conditions; type `./iRecovery -o' for details.\r\n" << endl;

	if (Device.Connect()) {
		
		Device.SendCommand("reboot");
		Device.Disconnect();
	}
	
	return 1;
}
