//============================================================================
// Name        : iRecovery.cpp
// Author      : GreySyntax
// Version     : 1.0
// Copyright   : GreySyntax 2010(C)
// Description : A C++ implementation of "iRecovery 2.x"
//============================================================================

#include <iostream>
#include "headers/Device.h"

using namespace std;

#define VERSION "1.0.2"

//Arguments
//-a auto-boot
//-c command
//-u upload
//-e execute
//-s screen
//-h help

int main() {
	cout << "iRecovery++ :: " << VERSION << "\r\n";
	cout << "by GreySyntax\r\n\r\n";
	cout << "Special thanks to posixninja\r\n" << endl;

	Device MobileDevice;

	if (! MobileDevice.Connect()) {
		cout << "[Device] Failed to Connect." << endl;
		return -1;
	}

	cout << "[Device] Connected." << endl;
	MobileDevice.Upload("README");
	MobileDevice.Disconnect();

	return 0;
}
