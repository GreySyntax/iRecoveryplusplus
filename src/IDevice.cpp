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

#include "IDevice.h"

using namespace std;

enum {
	kRecoveryMode	= 0x1281,
	kNormalMode		= 0x1290,
	kWTFMode		= 0x1227,
	kDFUMode		= 0x1222,
	kBufferSize		= 0x10000,
	kCommandMaxLen	= 0x200,
	kCommandTimeout = 500,
	kUploadTimeout	= 1000,
};

static char *deftext;

static int set_deftext() {
	
	if (deftext) {
		
		rl_insert_text(deftext);
		deftext = (char *) NULL;
		rl_startup_hook = (rl_hook_func_t *)NULL;
	}
	
	return 0; 
}

IDevice::IDevice() {
	
	//Stub
}

bool IDevice::AutoBoot() {
	
	if (! USB.IsConnected()) {
		Connect();
	}
}

bool IDevice::Connect() {
	
	if (USB.IsConnected()) {
		Disconnect();
	}
	
	if (! USB.Open(APPLE_VENDOR_ID, kRecoveryMode)) {
		
		cout << "[IDevice::Connect] Failed to connect to recovery." << endl;
		
		if (! USB.Open(APPLE_VENDOR_ID, kWTFMode)) {
			
			cout << "[IDevice::Connect] Failed to connect to wtf." << endl;
			
			if (! USB.Open(APPLE_VENDOR_ID, kDFUMode)) {
				
				cout << "[IDevice::Connect] Failed to connect to dfu." << endl;
				return false;
			}
		}
	}
	
	if (! USB.Configure(1)) {
		
		cout << "[IDevice::Connect] Failed to set usb configuration." << endl;
		Disconnect();
		return false;
	}
	
	if (! USB.ClaimInterface(0) || ! USB.ClaimInterface(1)) {
		
		cout << "[IDevice::Connect] Failed to claim interface's." << endl;
		//Disconnect();
		return false;
	}
	
	return true;
}

void IDevice::Disconnect() {
	
	if (! USB.IsConnected()) {
		return;
	}
	
	USB.Close();
	cout << "[IDevice::Disconnect] Closed connection." << endl;
}

bool IDevice::Exploit(const char* file) {
	
    return false;
    
    /*
     * I was going to add the 3.1.2 usb_control_msg(0x21) exploit here but,
     * i couldnt really find a reason todo so.
     */
    
    if (! USB.IsConnected()) {
		Connect();
	}
}

void IDevice::Reset() {
	
	if (! USB.IsConnected()) {
		Connect();
	}
	
	USB.Reset();
}

bool IDevice::SendCommand(const char* argv) {
	
	int length = strlen(argv);
	
	if (length > kCommandMaxLen) {
		
		cout << "[IDevice::SendCommand] Command to long, aborting." << endl;
		return false;
	}
	
	if (! USB.IsConnected()) {
		Connect();
	}
	
	if (! USB.Transfer(0x40, 0, 0, 0, argv, (length + 1), kCommandTimeout)) {
		
		cout << "[IDevice::SendCommand] Failed to send command." << endl;
		return false;
	}
	
	add_history(argv);
	write_history(LOG_FILE);
	
	char* action = strtok(strdup(argv), " ");
	
	if (! strcmp(action, "getenv")) {
		
		char response[0x200];
		USB.Transfer(0xC0, 0, 0, 0, response, 0x200, (kCommandTimeout * 2));
		cout << response << endl;
	}
	
	return true;
}

bool IDevice::SendBuffer(char* data, int length, int* actual_length) {
    
	if (! USB.IsConnected()) {
		Connect();
	}
    
	if (! USB.IsConnected() || ! USB.Configure(1) || ! USB.ClaimAltInterface(1, 1)) {
		
		return false;
	}
	
	if (USB.Write(0x04, data, length, actual_length, kUploadTimeout) != 0) {
		
		return false;
	}
	
	return true;
}

void IDevice::Shell() {
    
	if (! USB.IsConnected()) {
		Connect();
	}
    
    if (! USB.IsConnected() || ! USB.Configure(1) || ! USB.ClaimAltInterface(1, 1)) {
        
        return;
    }
    
    char* buffer = (char*)malloc(kBufferSize);
    
    if (buffer == NULL) {
		
        return;
    }
    
    //TODO Log to file
    
    bool runShell = true;
	int available = 0, pos = 0;
	char *prompt, *temp;

	deftext = (char *)0;
	
	if (deftext && *deftext) {
		
		rl_startup_hook = set_deftext;
	}
	
	read_history(LOG_FILE);
    
    while (runShell) {
        
		available = 0;
        
        memset(buffer, 0, kBufferSize);
        USB.Read(0x81, buffer, kBufferSize, &available, kCommandTimeout);
        
        if (available > 0) {
            
			for (pos = 0; pos < available; pos++) {
				
				cout << buffer[pos];
			}
			
            free(buffer);
            buffer = (char*)malloc(kBufferSize);
        }
		
		temp = readline("IDevice$ ");
		
		if (temp != 0 && temp && *temp) {
		
			//TODO Log
			
			if (temp[0] == '/' && strlen(temp) > 1 && temp[1] != ' ') {
				
				//TODO Handle command
				
			} else {
				
				SendCommand(temp);
			}
			
			//Ciao bitch!
			free(temp);
		}
    }
}

bool IDevice::Upload(const char* file) {
	
	FILE* data = fopen(file, "rb");
	
	if (data == NULL) {
		cout << "[IDevice::Upload] Failed to open file " << file << endl;
		return false;
	}
	
	cout << "[IDevice::Upload] Attemtping to upload file" << endl;
	
	fseek(data, 0, SEEK_END);
	unsigned int length = ftell(data);
	fseek(data, 0, SEEK_SET);
	
	char* buffer = (char*)malloc(length);
	
	if (buffer == NULL) {
		
		cout << "[IDevice::Upload] Failed to allocate " << length << " bytes" << endl;
		fclose(data);
		return false;
	}
	
	fread(buffer, 1, length, data);
	fclose(data);
	
	int actual_length = 0;//((int)length) + 1;
	
	return SendBuffer(buffer, length, &actual_length);
}
