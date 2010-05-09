//============================================================================
// Name        : Device.h
// Author      : GreySyntax
// Version     : 1.0
// Copyright   : GreySyntax 2010(C)
// Description : A C++ implementation of "iRecovery 2.x"
//============================================================================

#ifndef DEVICE_H_
#define DEVICE_H_

#define VENDOR_ID       0x05AC
#define NORM_MODE       0x1290
#define RECV_MODE       0x1281
#define WTF_MODE        0x1227
#define DFU_MODE        0x1222
#define BUF_SIZE        0x10000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libusb-1.0/libusb.h>

class Device {
public:
	Device();
	void AutoBoot();
	bool Buffer();//TODO
	bool Connect();
	void Disconnect();
	bool Exploit(char* file[]);//TODO
	void Init();
	void Reset();
	bool Send(char* argv);
	bool Upload(char* file[]);//TODO

private:
	struct libusb_device_handle *device;

};

#endif /* DEVICE_H_ */
